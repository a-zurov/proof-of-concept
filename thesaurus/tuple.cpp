// tuple.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
//

#include <iostream>
#include <cstring>
#include <type_traits>
#include <tuple>


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


template<class... _Types>
struct arg_tail
{
    std::tuple<_Types...> tuple_Args_;

    template<class... _TyArgs>
    arg_tail(_TyArgs&&... args)
        : tuple_Args_{ std::forward<_TyArgs>(args)... }
    {
        cout_dump();
    }
};

template<class _Ty, class... _Types>
struct arg_list
{
    _Ty arg1_;
    arg_tail<_Types...> tail_Args_;

    template<class T, class... _TyArgs>
    arg_list(T arg1, _TyArgs&&... args)
        : arg1_{ std::forward<T>(arg1) }
        , tail_Args_{ std::forward<_TyArgs>(args)... }
    {
        cout_dump();
    }
};

template<class T, class... _TyArgs>
arg_list<T, _TyArgs...> create_arg_list(T arg1, _TyArgs&&... args)
{
    cout_dump();
    return arg_list<T, _TyArgs...>{ std::forward<T>(arg1), std::forward<_TyArgs>(args)...};
}

struct S {
    std::string s_;
    ~S() {
        cout_dump_msg(s_);
    }
    S(const std::string s) : s_(s) {
        cout_dump_msg(s_);
    }
    S(const S& other) : s_(other.s_) {
        cout_dump_msg(s_ << ' ' << other.s_);
    }
    S(S&& other) noexcept : s_(std::move(other.s_)) {
        cout_dump_msg(s_ << ' ' << other.s_);
    }
};

template <typename T>
void printHelper() {
    std::cout << "unknown\n";
}
template <>
void printHelper<int>() {
    std::cout << "int\n";
}
template <>
void printHelper<int&>() {
    std::cout << "int&\n";
}
template <>
void printHelper<int*>() {
    std::cout << "int*\n";
}
template <>
void printHelper<S>() {
    std::cout << "S\n";
}
template <>
void printHelper<S&>() {
    std::cout << "S&\n";
}

template<std::size_t Index, class T>
void printTypeAtIndex()
{
    std::cout << "index " << Index << " has type: ";
    using SelectedType = std::tuple_element_t<Index, T>;
    printHelper<SelectedType>();
}

int main(int argc, char** argv) {

    S s{ "abc" };
    int x = 10;
    auto h = create_arg_list(0, &x, x, 200, s, S{"xyz"});

    printTypeAtIndex<0, decltype(h.tail_Args_.tuple_Args_)>();
    printTypeAtIndex<1, decltype(h.tail_Args_.tuple_Args_)>();
    printTypeAtIndex<2, decltype(h.tail_Args_.tuple_Args_)>();
    printTypeAtIndex<3, decltype(h.tail_Args_.tuple_Args_)>();
    printTypeAtIndex<4, decltype(h.tail_Args_.tuple_Args_)>();
}
