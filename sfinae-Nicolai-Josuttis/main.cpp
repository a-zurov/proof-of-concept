// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// Inspired by CppCon 2017: Nicolai Josuttis “The Nightmare of Move Semantics for Trivial Classes”:
// https://www.youtube.com/watch?v=PNRju6_yn3o
// Presentation Slides, PDFs, Source Code and other presenter materials are available at:
// https://github.com/CppCon/CppCon2017
// ------------------------------------------------------------------------------------------------

#include <iostream>
#include "macro.h"
#include "String.h"

static constexpr ccn::conststr const& g_CxxVersionName(ccn::getCxxVersionName());

using String = ccn::String;

#ifdef _MSC_VER
#if ( _Cxx_14 <= __CXX_VER__ )
#define _IS_CONVERTIBLE
#endif
#else
#if ( _Cxx_17 <= __CXX_VER__ )
#define _IS_CONVERTIBLE
#endif
#endif

class Cust {

    String  m_szFirst;
    String  m_szLast;
    int     m_nID = 0;

public:
    Cust() = default;

    template< class T
        , typename = std::enable_if_t < std::is_same<T, String>::value >
    > void foo(T s)
    {
        cout_dump_msg(s);
    }

    template< class T
        , typename = std::enable_if_t< std::is_same<T, String>::value >
    > void boo(T&& s)
    {
        cout_dump_msg(std::forward<T>(s));
    }

#ifdef _IS_CONVERTIBLE

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< std::is_convertible_v<S1, String> >
    > Cust(S1&& first, S2&& last = "", int id = 0)
        : m_szFirst(std::forward<S1>(first))
        , m_szLast(std::forward<S2>(last))
        , m_nID(id) {}

#else

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< !std::is_same<S1, Cust&>::value >
    > Cust(S1&& first, S2&& last = "", int id = 0)
        : m_szFirst(std::forward<S1>(first))
        , m_szLast(std::forward<S2>(last))
        , m_nID(id) {}

#endif

};


int main()
{

    std::cout << "Hello Move Semantics with " << g_CxxVersionName << "!\n";

    checkpoint(0);
    {
        cout_dump();
        cout_dump_msg("Ish govna kakya!!");
    }

    checkpoint(1);
    {
        String szFirst;
        String szSecond("xyz");
        std::cout << szFirst << " " << szSecond << std::endl;
        szFirst = szSecond;
        std::cout << szFirst << " " << szSecond << std::endl;
        szFirst = std::move(szSecond);
        std::cout << szFirst << " " << szSecond << std::endl; // used a moved object
    }

    checkpoint(3);
    {
        Cust a;
        String x("Bob");
        const String& y = x;
        a.foo(std::move(String("Tim")));
        a.foo(x);
        a.foo(y); // by value

        a.boo(String("Rob"));
        a.boo(std::move(String("Den")));
        a.boo(std::move(x));
    }

    checkpoint(4);
    {
        Cust c{ "Joe", "Fix", 42 };
        Cust f{ "Nico" };
        Cust g{ f };
        const Cust cc{ "Jim" };
        Cust h{ cc };
    }
}
