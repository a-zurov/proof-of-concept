// invoke.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
//

#include <iostream>
#include <cstring>
#include <cassert>
#include <type_traits>
#include <functional>

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

#if 0 // invoke_switch
#ifdef _MSC_VER
#define __invoke__ std::invoke
#else
#define __invoke__ std::__invoke
#endif

template<typename F, class... _Types>
inline void foo(F f, _Types&&... args) {
    __invoke__(f, std::forward<_Types>(args)...);
}

#undef  __invoke__
#else // invoke_switch

template<typename F, class... _Types>
auto foo(F f, _Types&&... args) -> std::enable_if_t<
    not std::is_member_function_pointer<F>::value
>
{
    cout_dump();
    f(std::forward<_Types>(args)...);
}

template<typename F, class _Ty1, class... _Types2>
auto foo(F f, _Ty1 arg1, _Types2... args) -> std::enable_if_t<
    std::is_member_function_pointer<F>::value and not std::is_pointer<_Ty1>::value
>
{
    cout_dump();
    (arg1.*f)(std::forward<_Types2>(args)...);
}

template<typename F, class _Ty1, class... _Types2>
auto foo(F f, _Ty1 arg1, _Types2... args) -> std::enable_if_t <
    std::is_member_function_pointer<F>::value and std::is_pointer<_Ty1>::value
>
{
    cout_dump();
    (arg1->*f)(std::forward<_Types2>(args)...);
}

#endif // invoke_switch

void bar(int x1, int x2, int x3) {
    cout_dump_msg(x1 << ' ' << x2 << ' ' << x3);
}

struct Bar
{
    void bar(int x1, int x2, int x3) {
        cout_dump_msg(x1 << ' ' << x2 << ' ' << x3);
    }

    void operator()(int j, const std::string& s) {
        cout_dump_msg(j << ' ' << s);
    }
};

int main(int argc, char** argv) {

    Bar b;
    int x1{ 1 }, x2{ 2 };

    foo(&bar, x1, x2, 3);
    foo(&Bar::bar, b, x2, 3, x1);
    foo(&Bar::bar, &b, 3, x1, x2);

    foo(&Bar::operator(), b, 9, "abc");
}
