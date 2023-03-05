// bind.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
//

#include <iostream>
#include <cstring>
#include <type_traits>
#include <functional>
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

//#define ALLOW_DUMP
#ifdef ALLOW_DUMP
#define cout_dump() std::cout << __MAKE_DUMP__ << '\n'
#else
#define cout_dump()
#endif


template<class... _Types>
struct argument_list
{
    std::tuple<_Types...> tupleArgs_;

    template<class... T>
    constexpr argument_list(T&&... args) noexcept
        : tupleArgs_{ std::forward<T>(args)... }
    {
        cout_dump();
    }

    template<size_t n>
    constexpr decltype(auto) operator[](std::integral_constant<size_t, n>) noexcept {
        cout_dump();
        return std::get<n>(tupleArgs_);
    }
};

template<class... _Types>
struct callee_list
{
    std::tuple<_Types&&...> tupleArgs_;

    template<class... _TyArgs>
    constexpr callee_list(_TyArgs&&... args) noexcept
        : tupleArgs_{ std::forward<_TyArgs>(args)... }
    {
        cout_dump();
    }

    template< class T,
        typename = std::enable_if_t< std::is_placeholder<std::remove_reference_t<T>>::value != 0 >
    >
    constexpr decltype(auto) operator[](T) noexcept
    {
        cout_dump();
        return std::get<std::is_placeholder<T>::value - 1>(std::move(tupleArgs_));
    }

    template < typename T
        , typename = std::enable_if_t<
            std::is_placeholder<std::remove_reference_t<T>>::value == 0
        >
    >
    constexpr decltype(auto) operator[](T&& t) noexcept
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
    constexpr binder(_Fn&& f, _TyArgs&&... args) noexcept
        : f_{ std::forward<_Fn>(f) }
        , argumentList_{ std::forward<_TyArgs>(args)...}
    {
        cout_dump();
    }

    template<class ... _TyArgs>
    constexpr decltype(auto) operator()(_TyArgs&&... args)
    {
        return call(std::make_index_sequence<sizeof...(_Types)>{}, static_cast<_TyArgs&&>(args)...);
    }

private:

    template<class ... _TyArgs, size_t ... Seq>
    constexpr decltype(auto) call(std::index_sequence<Seq...>, _TyArgs&&... args)
    {
        return f_(
                    (
                        callee_list<_TyArgs...>{ static_cast<_TyArgs&&>(args)...} [
                            argumentList_[ std::integral_constant<size_t, Seq>{} ]
                        ]
                    )...
                );
    }
};

template<class F, class ... _Types>
binder<F, _Types...> bind2(F&& f, _Types&&... args)
{
    return binder<F, _Types...>{ std::forward<F>(f), std::forward<_Types>(args)...};
}

void foo(const std::string& str) {
    cout_dump_msg(str);
}

int sum(int a, int b) {
    return a + b;
}

using namespace std::placeholders;

int main(int argc, char** argv) {

    std::function<void(const std::string&)> f = bind2(foo, _1);
    f("xyz");

    cout_dump_msg(bind2(sum, 1, 2)());
    cout_dump_msg(bind2(sum, _1, _2)(20, 30));
}
