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
    assert((std::is_same< decltype(arg), typename std::remove_reference<T>::type& >::value));
}

void ops(int j) {
    cout_dump_msg(j);
}

std::string& foo(std::string& s) {
    return s;
}

auto bar1(std::string& s)
{
    return foo(s);
}

auto bar2(std::string& s) -> decltype(foo(s))
{
    return foo(s);
}

decltype(auto) bar3(std::string& s)
{
    return foo(s);
}

// move-semantics --------------------------------------

template<typename U>
U&& forward(std::remove_reference_t<U>& x) noexcept {
    return static_cast<U&&>(x);
}

template <typename W>
struct S {
    W w_;

    template< typename T >
    S(T&& arg) : w_(static_cast<W>(arg)) {
        cout_dump();
    }
};

template< typename T >
void l_value_no_assert(T&& arg) {
    cout_dump();
    // category(arg) = l-value
    // type(arg) = type
    // -----------------------------------
    // inside template:
    // deducted(T) = type &
    assert((std::is_same< T, std::remove_reference_t<T>& >::value));
    // signature(T&& + deducted(T)) = T (&& + &) = type &
    assert((std::is_same< decltype(static_cast<T&&>(arg)), std::remove_reference_t<T>& >::value));
    // decltype(arg) = type &
    assert((std::is_same< decltype(arg), std::remove_reference_t<T>& >::value));
    // inside forward (it's signature is not a universal reference):
    // U = deducted(T) = type &
    // forward(std::remove_reference_t<type &>& arg) = forward(type & arg)
    // static_cast<U&&>(x) = static_cast<type(&& + &)>(x) = type & arg
    // -----------------------------------
    // in S-ctor signature:
    // category(forward<T>(arg)) = r-value
    // type(forward<T>(arg)) = type &
    S<T&&> s_lref_w(forward<T>(arg));
    S<std::remove_reference_t<T>&&> s_rref_w(forward<T>(arg));
    assert((not std::is_same<decltype(s_lref_w.w_), decltype(s_rref_w.w_)>::value));
}

template< typename T>
void r_value_no_assert(T&& arg) {
    cout_dump();
    // category(arg) = r-value
    // type(arg) = type
    // -----------------------------------
    // inside template:
    // deducted(T) = type
    assert((std::is_same< T, std::remove_reference_t<T> >::value)); // the main crutch
    // signature(T&& + deducted(T)) = T (&& + 0) = type &&
    assert((std::is_same< decltype(static_cast<T&&>(arg)), std::remove_reference_t<T>&& >::value));
    // decltype(arg) = type &&
    assert((std::is_same< decltype(arg), std::remove_reference_t<T>&& >::value));
    // inside forward (it's signature is not a universal reference):
    // U = deducted(T) = type
    // forward(std::remove_reference_t<type>& arg) = forward(type & arg)
    // static_cast<U&&>(x) = static_cast<type(&& + 0)>(x) = type && arg
    // -----------------------------------
    // in S-ctor signature:
    // category(forward<T>(arg)) = r-value
    // type(forward<T>(arg)) = type &&
    S<T&&> s_rref1_w(forward<T>(arg));
    S<std::remove_reference_t<T>&&> s_rref2_w(forward<T>(arg));
    assert((std::is_same<decltype(s_rref1_w.w_), decltype(s_rref2_w.w_)>::value));
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


    std::string s("abc");

    auto s1 = bar1(s);
    assert((std::is_same< decltype(s1), std::string>::value));

    decltype(auto) s2 = bar2(s);
    auto s2_auto = bar2(s);
    assert((std::is_same< decltype(s2), std::string&>::value));
    assert((std::is_same< decltype(s2_auto), std::string>::value));

    decltype(auto) s3 = bar3(s);
    auto s3_auto = bar3(s);
    assert((std::is_same< decltype(s3), std::string&>::value));
    assert((std::is_same< decltype(s3_auto), std::string>::value));

    int k = 3;
    l_value_no_assert(k);
    //l_value_no_assert(3);
    //r_value_no_assert(k);
    r_value_no_assert(3);
}
