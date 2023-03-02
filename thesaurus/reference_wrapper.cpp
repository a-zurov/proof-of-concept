// reference_wrapper.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
//

#include <iostream>
#include <cstring>
#include <cassert>
#include <ctime>
#include <vector>
#include <algorithm>

#if defined( _MSC_VER )
#define __PRETTY_FUNCTION__ __FUNCSIG__
#define __DELIM__ '\\'
#else
#define __DELIM__ '/'
#endif
#define __FILENAME__ ( std::strrchr( "/" __FILE__, __DELIM__ ) + 1 )
#define __MAKE_DUMP__ __FILENAME__ << " : " << __LINE__ << " | " << __PRETTY_FUNCTION__

#define cout_dump_msg(x) std::cout << __MAKE_DUMP__ << ' ' << x << '\n'

//#define ALLOW_DUMP
#ifdef ALLOW_DUMP
#define cout_dump() std::cout << __MAKE_DUMP__ << '\n'
#else
#define cout_dump()
#endif

template<class T>
T* addressof(T& arg)
{
    return reinterpret_cast<T*>(
        &const_cast<char&>(reinterpret_cast<const volatile char&>(arg))
        );
}

template<typename T>
struct reference_wrapper
{
    T* m_ptr;

    reference_wrapper(T& t) : m_ptr(addressof(t)) {
        cout_dump();
    }

    reference_wrapper(const reference_wrapper<T>& other) : m_ptr(other.m_ptr) {
        cout_dump();
    }

    reference_wrapper& operator=(const reference_wrapper<T>& rhs) {
        cout_dump();
        m_ptr = rhs.m_ptr;
        return *this;
    }

    operator T& () const {
        cout_dump();
        return this->get();
    }

    T& get() const {
        cout_dump();
        return *m_ptr;
    }

    template <typename U>
    bool operator()(const U& lhs, const U& rhs) {
        cout_dump();
        return m_ptr->operator()(lhs, rhs);
    }
};

template<typename T>
struct B {

    T* m_ptr;

    B(T& t) : m_ptr(&t) {
        cout_dump();
    }

    B(const B<T>& other) : m_ptr(other.m_ptr) {
        cout_dump();
    }

    B& operator=(const B<T>& rhs) {
        cout_dump();
        m_ptr = rhs.m_ptr;
        return *this;
    }

    T* operator &() const {
        cout_dump();
        return m_ptr;
    }
};

template <typename T>
struct type_cast_predicate
{
    static int ms_fcall_count;

    static bool predicate(const T& lhs, const T& rhs) {
        ++ms_fcall_count;
        return lhs > rhs;
    }

    using func_ptr_t = bool (*)(const T& lhs, const T& rhs);

    operator func_ptr_t() const {
        return predicate;
    }
};

template <typename T>
int type_cast_predicate<T>::ms_fcall_count = 0;

template <typename T>
struct functor_predicate
{
    static int ms_ctor_count;
    static int ms_cpytor_count;
    static int ms_assop_count;
    static int ms_fcall_count;

    int m_fcall_count{};

    functor_predicate() {
        cout_dump();
        ++ms_ctor_count;
    }

    functor_predicate(const functor_predicate<T>& other) {
        cout_dump();
        ++ms_cpytor_count;
    }

    functor_predicate& operator=(const functor_predicate<T>& rhs) {
        cout_dump();
        ++ms_assop_count;
        return *this;
    }

    bool operator()(const T& lhs, const T& rhs) {
        ++m_fcall_count;
        ++ms_fcall_count;
        return lhs > rhs;
    }
};

template <typename T>
int functor_predicate<T>::ms_ctor_count = 0;

template <typename T>
int functor_predicate<T>::ms_cpytor_count = 0;

template <typename T>
int functor_predicate<T>::ms_assop_count = 0;

template <typename T>
int functor_predicate<T>::ms_fcall_count = 0;

template <typename T>
using vector = std::vector<T>;

int main(int argc, char** argv) {

    vector<int> vec = { 7, 3, 8, 5, 4, 6, 1, 9, 2 };

    if (argc != 2) {
        printf("can use input parameter\t: %s <number-of-elements>\n",
            std::strrchr(argv[0], __DELIM__) + 1);
    } else {
        int N = strtol(argv[1], NULL, 10);
        assert(N > 0);
        printf("input parameter\t: %d \n", N);
        vec.resize(N);
        for (size_t j = 0; j < N; ++j) {
            vec[j] = rand() % N;
        }
    }

    clock_t start, finish;

    vector<reference_wrapper<int>> vec_rw0{ vec.begin(), vec.end() };

    start = clock();
    std::sort(vec_rw0.begin(), vec_rw0.end(), type_cast_predicate<int>());
    finish = clock();
    cout_dump_msg(type_cast_predicate<int>::ms_fcall_count << " time = " << (double)(finish - start) / CLOCKS_PER_SEC);

    functor_predicate<int> fpred;

    vector<reference_wrapper<int>> vec_rw1{ vec.begin(), vec.end() };

    start = clock();
    std::sort(vec_rw1.begin(), vec_rw1.end(), fpred);
    finish = clock();
    cout_dump_msg(fpred.ms_fcall_count << " time = " << (double)(finish - start) / CLOCKS_PER_SEC );
    assert(0 == fpred.m_fcall_count);

    vector<reference_wrapper<int>> vec_rw2{ vec.begin(), vec.end() };

    start = clock();
#ifdef _MSC_VER
    std::sort(vec_rw2.begin(), vec_rw2.end(), reference_wrapper(fpred));
#else
    std::sort(vec_rw2.begin(), vec_rw2.end(), reference_wrapper<functor_predicate<int>>(fpred));
#endif
    finish = clock();
    cout_dump_msg(fpred.m_fcall_count << " time = " << (double)(finish - start) / CLOCKS_PER_SEC);
    assert(type_cast_predicate<int>::ms_fcall_count == fpred.m_fcall_count);

    int x = 5;

#ifdef _MSC_VER
    auto a = reference_wrapper(x);
    auto b = B(x);
    auto c = reference_wrapper(b);
#else
    auto a = reference_wrapper<int>(x);
    auto b = B<int>(x);
    auto c = reference_wrapper<B<int>>(b);
#endif

}
