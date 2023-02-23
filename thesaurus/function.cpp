// function.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
// std::function<bool(int, char*)>

#include <iostream>
#include <cstring>
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

struct function
{
    std::unique_ptr<callable_base> m_callable;

    template<typename T>
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

int main() {

    function f = foo;
    f(10, const_cast<char*>("xyz"));
}