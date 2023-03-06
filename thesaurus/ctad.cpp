// ctad.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
//

#include <iostream>
#include <cstring>
#include <cassert>
#include <type_traits>

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


template<typename T>
void copy_init_arg(T arg) {
    cout_dump();
    assert((std::is_same< decltype(arg), typename std::remove_reference<T>::type >::value));
}

template<typename T>
void byref_init_arg(T& arg) {
    cout_dump();
    assert((not std::is_same< decltype(arg), typename std::remove_reference<T>::type >::value));
}

void ops(int j) {
    cout_dump_msg(j);
}

int main() {

    int x = 27;
    const int cx = x;
    const int& cref_x = x;

    const char* const ptr = "abc";

    copy_init_arg(ops);

    copy_init_arg(x);
    copy_init_arg(cx);
    copy_init_arg(cref_x);

    copy_init_arg(ptr);

    byref_init_arg(ops);

    byref_init_arg(x);
    byref_init_arg(cx);
    byref_init_arg(cref_x);

    byref_init_arg(ptr);
}