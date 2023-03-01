// addressof.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
//

#include <iostream>
#include <cstring>

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
struct A
{
    T* m_ptr;

    A(T& t) : m_ptr(addressof(t)) {
        cout_dump();
    }

    A(const A<T>& other) : m_ptr(other.m_ptr) {
        cout_dump();
    }

    A& operator=(const A<T>& rhs) {
        cout_dump();
        m_ptr = rhs.m_ptr;
        return *this;
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

int main(int argc, char** argv) {

    int x = 5;

#ifdef _MSC_VER
    auto a = A(x);
    auto b = B(x);
    auto c = A(b);
#else
    auto a = A<int>(x);
    auto b = B<int>(x);
    auto c = A<B<int>>(b);
#endif

}
