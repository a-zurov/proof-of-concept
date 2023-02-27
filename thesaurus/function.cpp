// function.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
// std::function<bool(int, char*)>

#include <iostream>
#include <cstring>
#include <cassert>
#include <type_traits>
#include <memory>

#if defined( _MSC_VER )
#define __PRETTY_FUNCTION__ __FUNCSIG__
#define __DELIM__ '\\'
#else
#define __DELIM__ '/'
#endif
#define __FILENAME__ ( std::strrchr( "/" __FILE__, __DELIM__ ) + 1 )
#define __MAKE_DUMP__ __FILENAME__ << " : " << __LINE__ << " | " << __PRETTY_FUNCTION__

#define cout_dump() std::cout << __MAKE_DUMP__ << '\n'
#define cout_dump_msg(x) std::cout << __MAKE_DUMP__ << ' ' << x << '\n'


struct callable_base
{
    callable_base() = default;
    virtual ~callable_base() { }
    virtual bool invoke(int, char*) = 0;
    virtual std::unique_ptr<callable_base> clone() = 0;
};

template<typename T>
struct callable : callable_base
{
    T m_t;

    callable(T const& t) : m_t(t) {
        cout_dump();
    }

    callable(T&& t) : m_t(std::move(t)) {
        cout_dump();
    }

    bool invoke(int j, char* pch) override {
        cout_dump();
        return m_t(j, pch);
    }

    std::unique_ptr<callable_base> clone() override {
        cout_dump();
        return std::make_unique<callable>(m_t);
    }
};

template<typename A, typename B>
using disable_if_same_or_derived =
typename std::enable_if<
    !std::is_base_of<A, typename  std::remove_reference<B>::type>::value
    >::type;

struct function
{
    std::unique_ptr<callable_base> m_callable;

    template<typename T, typename = disable_if_same_or_derived<function,T>>
    function(T&& t) : m_callable{ new callable<std::decay_t<T>>( std::forward<T>(t) ) } {
        cout_dump();
    }

    function(const function& other) :
        m_callable{ other.m_callable ? other.m_callable->clone() : nullptr } {
        cout_dump();
    }

    function(function&& other) = default;

    bool operator()(int a, char* b)
    {
        // TODO: bad_function_call exception
        cout_dump();
        return m_callable->invoke(a, b);
    }
};

bool foo(int j, char* pch) {
    cout_dump_msg(j << ' ' << pch);
    return true;
}

template <typename T>
using decay_t = std::decay_t<T>;

template <typename T>
void boo(decay_t<T>) {
    cout_dump();
}

struct S {
    S() { cout_dump(); }
    S(const S&) { cout_dump(); }
};

void goo(S) {
    cout_dump();
}

bool ops(function f) {
    cout_dump();
    return f(5, const_cast<char*>("abc"));
}

int main() {

    S s;
    goo(s);

    using func_t = bool(int, char*);
    using ptr_func_t = bool(*)(int, char*);
    using ref_func_t = bool(&)(int, char*);
    using ref_ptr_func_t = bool(*&)(int, char*);
    using rref_ptr_func_t = bool(*&&)(int, char*);

    boo<func_t>(foo);

    assert((std::is_same< decay_t<func_t>, decay_t<ptr_func_t> >::value));
    assert((std::is_same< decay_t<func_t>, decay_t<ref_func_t> >::value));
    assert((std::is_same< decay_t<func_t>, decay_t<ref_ptr_func_t> >::value));
    assert((std::is_same< decay_t<func_t>, decay_t<rref_ptr_func_t> >::value));

    function f = foo;
    f(10, const_cast<char*>("xyz"));

    ops(f);
}