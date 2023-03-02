// reference_wrapper.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
//

#include <iostream>
#include <cstring>
#include <cassert>
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

#define ALLOW_DUMP
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
    static int m_count;

    static bool predicate(const T& lhs, const T& rhs) {
        ++m_count;
        return lhs > rhs;
    }

    using func_ptr_t = bool (*)(const T& lhs, const T& rhs);

    operator func_ptr_t() const {
        return predicate;
    }
};

template <typename T>
int type_cast_predicate<T>::m_count = 0;

template <typename T>
struct functor_predicate
{
    int m_count{};

    bool operator()(const T& lhs, const T& rhs) {
        ++m_count;
        return lhs > rhs;
    }
};

template <typename T>
using vector = std::vector<T>;

int main(int argc, char** argv) {

    int x = 5;

    vector<int> vec = { 7, 3, 8, 5, 4, 6, 1, 9, 2 };

    vector<reference_wrapper<int>> vec_rw0{ vec.begin(), vec.end() };
    std::sort(vec_rw0.begin(), vec_rw0.end(), type_cast_predicate<int>());
    cout_dump_msg(type_cast_predicate<int>::m_count);

    functor_predicate<int> fpred;

    vector<reference_wrapper<int>> vec_rw1{ vec.begin(), vec.end() };
    std::sort(vec_rw1.begin(), vec_rw1.end(), fpred);
    assert(0 == fpred.m_count);

#ifdef _MSC_VER

    auto a = reference_wrapper(x);
    auto b = B(x);
    auto c = reference_wrapper(b);

    vector<reference_wrapper<int>> vec_rw2{ vec.begin(), vec.end() };
    std::sort(vec_rw2.begin(), vec_rw2.end(), reference_wrapper(fpred));
    assert(type_cast_predicate<int>::m_count == fpred.m_count);

#else

    auto a = reference_wrapper<int>(x);
    auto b = B<int>(x);
    auto c = reference_wrapper<B<int>>(b);

    vector<reference_wrapper<int>> vec_rw2{ vec.begin(), vec.end() };
    std::sort(vec_rw2.begin(), vec_rw2.end(), reference_wrapper<functor_predicate<int>>(fpred));
    assert(type_cast_predicate<int>::m_count == fpred.m_count);

#endif

}
