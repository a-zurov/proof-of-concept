// bind.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
//

#include <iostream>
#include <cstring>
#include <type_traits>
#include <functional>
#include <tuple>
#include <vector>

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


template<class... _Types>
struct argument_list
{
    std::tuple<_Types...> tuple_Args_;

    template<class... T>
    argument_list(T&&... args) noexcept
        : tuple_Args_{ std::forward<T>(args)... }
    {
        cout_dump();
    }

    template<size_t n>
    decltype(auto) operator[](std::integral_constant<size_t, n>) noexcept
    {
        cout_dump();
        return std::get<n>(tuple_Args_);
    }
};

template<class... _Types>
struct rref_arg_list
{
    std::tuple<_Types&&...> tuple_rref_Args_;

    template<class... _TyArgs>
    rref_arg_list(_TyArgs&&... args) noexcept
        : tuple_rref_Args_( std::forward<_TyArgs>(args)...)
    {
        cout_dump();
    }

    template< class T,
        typename = std::enable_if_t<
        (std::is_placeholder<std::remove_reference_t<T>>::value != 0)
        >
    >
    decltype(auto) operator[](T) noexcept
    {
        cout_dump();
        return std::get<std::is_placeholder<T>::value - 1>(/*NB(!)*/ std::move(tuple_rref_Args_));
    }

    template < typename T
        , typename = std::enable_if_t<
            (std::is_placeholder<std::remove_reference_t<T>>::value == 0)
        >
    >
    T&& operator[](T&& t) noexcept // forward
    {
        cout_dump();
        return static_cast<T&&>(t);
    }
};

template<class F, class ... _Types>
struct binder
{
    std::function<std::remove_reference_t<std::remove_pointer_t<F>>> f_;

    argument_list<_Types...> argumentList_;

    template<class _Fn, class ... _TyArgs>
    binder(_Fn f, _TyArgs&&... args) noexcept
        : f_{ f }
        , argumentList_{ std::forward<_TyArgs>(args)...}
    {
        cout_dump();
    }

    template<class ... _TyArgs>
    decltype(auto) operator()(_TyArgs&&... args)
    {
        cout_dump();
        return call( std::make_index_sequence<sizeof...(_Types)>{}, std::forward<_TyArgs>(args)...);
    }

private:

    template<class ... _TyArgs, size_t ... Seq>
    decltype(auto) call( std::index_sequence<Seq...> , _TyArgs&&... args)
    {
        cout_dump();
        return f_(
                    (
                        rref_arg_list<_TyArgs...>{/*NB(!)*/ std::move(args)... }.operator[] (
                            argumentList_[ std::integral_constant<size_t, Seq>{} ]
                            )
                    )...
                );
    }
};

template<class F, class ... _Types>
binder<F, _Types...> bind2(F f, _Types&&... args)
{
    return binder<F, _Types...>{ f, std::forward<_Types>(args)...};
}

struct S {
    std::string s_;
    S() { cout_dump(); }
    ~S() { cout_dump(); }
    S(const S& other) : s_(other.s_) {
        cout_dump();
    }
    S(S&& other) noexcept : s_(std::move(other.s_))  {
        cout_dump();
    }
};

template<typename T>
T foo(T&& x, const std::string& s) {

    if (std::is_same<T, S>::value) {
        cout_dump_msg(s);
        x.s_ = s;
    } else cout_dump();
    return x;
}

void foo1(int j) {
    cout_dump_msg(j);
}

void foo2(int j, const std::string& str) {
    cout_dump_msg(j << ' ' << str);
}

int sum(int a, int b) {
    return a + b;
}

using namespace std::placeholders;

int main(int argc, char** argv) {

    std::vector<S> v;

    auto g = std::bind(foo<S>, _1, "abc");
    v.emplace_back(std::move(g(S{})));

    auto f = bind2(foo<S>, _1, "xyz");
    S s;
    f(std::move(s));
    v.emplace_back(std::move(f(S{})));
    v.emplace_back(std::move(f(S{})));

    for (const auto& elem : v) {
        cout_dump_msg(elem.s_);
    }

    bind2(foo1, 1)();

    std::function<void(int)> f1 = bind2(foo1, _1);
    f1(100);

    std::function<void(int, const std::string&)> f2 = bind2(foo2, 42, _2);
    f2(500, "xyz");

    std::function<void(int, const std::string&)> f3 = bind2(foo2, _1, _2);
    f3(500, "abc");

    cout_dump_msg(bind2(sum, 2, 3)());
    cout_dump_msg(bind2(sum, _1, _2)(20, 30));
}
