#pragma once

#include "String.h"

#ifdef _MSC_VER
#if ( _Cxx_14 <= __CXX_VER__ )
#define _IS_CONVERTIBLE
#endif
#else
#if ( _Cxx_17 <= __CXX_VER__ )
#define _IS_CONVERTIBLE
#endif
#endif

using String = cxx::String;

class Customer {

    String  m_szFirst;
    String  m_szLast;
    int     m_nID = 0;

public:
    Customer() = default;

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
    > Customer(S1&& first, S2&& last = "", int id = 0)
        : m_szFirst(std::forward<S1>(first))
        , m_szLast(std::forward<S2>(last))
        , m_nID(id) {}

#else

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< !std::is_same<S1, Customer&>::value >
    > Customer(S1&& first, S2&& last = "", int id = 0)
        : m_szFirst(std::forward<S1>(first))
        , m_szLast(std::forward<S2>(last))
        , m_nID(id) {}

#endif //_IS_CONVERTIBLE

};

void Checkpoint_0X() {

    checkpoint(0X_Begin);
    {
        String x("Bob");
        const String& y = x;
        Customer a;
        a.foo(std::move(String("Tim")));
        a.foo(x);
        a.foo(y); // by value

        a.boo(String("Rob"));
        a.boo(std::move(String("Den")));
        a.boo(std::move(x));

        checkpoint(0X_End);
    }
}

void Checkpoint_0Z() {

    checkpoint(0Z_Begin);
    {
        Customer c{ "Joe", "Fix", 42 };
        Customer f{ "Nico" };
        Customer g{ f };
        const Customer cc{ "Jim" };
        Customer h{ cc };

        checkpoint(0Z_End);
    }
}

