#pragma once

#include <type_traits>

#include "macro.h"
#include "String.h"

template <typename T>
typename std::remove_reference<T>::type && move(T&& x) {
    cout_dump();
    return static_cast< typename std::remove_reference<T>::type && >(x);
}

using String = cxx::String;


struct StringsBox {

    String  m_szFirst;
    String  m_szLast;

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< std::is_convertible_v<S1, String> >
    >StringsBox(S1&& first, S2&& last = "")
        : m_szFirst( std::forward<S1>(first) )
        , m_szLast( std::forward<S2>(last) ) {}

    void print() {
        std::cout << m_szFirst << " " << m_szLast << '\n';
    }
};

struct CopyBox : public StringsBox {

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< std::is_convertible_v<S1, String> >
    > CopyBox(S1&& first, S2&& last = "")
        : StringsBox( first
                    , last ) {}
};

struct MoveBox : public StringsBox {

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< std::is_convertible_v<S1, String> >
    > MoveBox(S1&& first, S2&& last = "")
        : StringsBox( std::is_same< decltype(first), typename std::remove_reference<S1>::type&& >::value ? move(first) : first
                    , std::is_same< decltype(last), typename std::remove_reference<S2>::type&& >::value ? move(last) : last ) {}
};

struct GreedyBox : public StringsBox {

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< std::is_convertible_v<S1, String> >
    > GreedyBox(S1&& first, S2&& last = "")
        : StringsBox( move(first)
                    , move(last) ) {}
};


template<typename T>
void f(T&& y) {

    std::cout << ( std::is_same< decltype(y), typename std::remove_reference<T>::type&& >::value ? "decltype(y) = T &&" :
                    ( std::is_same< decltype(y), typename std::remove_reference<T>::type& >::value ? "decltype(y) = T &" : "decltype(y) = T" )
                 );

    std::cout << '\n';
}

void Checkpoint_04()
{
    checkpoint(04_Begin);
    {

        f(0);

        int y = 0;
        f(y);

        int& ref_y = y;
        f(ref_y);

        int&& rref_y = move(y);
        f(rref_y);

        checkpoint(04_A);

        int x = 1;

        int&& rref_x = move(x);
        int& ref_rref_x = rref_x;

        ++rref_x;

        std::cout << x << rref_x << ref_rref_x << '\n';

        int&& rref_rref_x = move(rref_x);
        int& ref_rref_rref_x = rref_rref_x;

        ref_rref_rref_x = 0;

        std::cout << x << rref_x << ref_rref_x << rref_rref_x << ref_rref_rref_x << '\n';

        checkpoint(04_B);

        String str1 = "cde";
        String str2 = str1;

        checkpoint(04_B_CopyBox);

        CopyBox cb1("abc", str1);
        cb1.print();
        std::cout << str1 << '\n';

        CopyBox cb2("abc", move(str1));
        cb2.print();
        std::cout << str1 << '\n';

        checkpoint(04_B_MoveBox);

        MoveBox mb1("abc", str1);
        mb1.print();
        std::cout << str1 << '\n';

        MoveBox mb2("abc", move(str1));
        mb2.print();
        std::cout << str1 << '\n';

        checkpoint(04_B_GreedyBox);

        GreedyBox gb1("abc", str2);
        gb1.print();
        std::cout << str2 << '\n';

        GreedyBox gb2("abc", move(str2));
        gb2.print();
        std::cout << str2 << '\n';

        checkpoint(04_End);
    }
}
