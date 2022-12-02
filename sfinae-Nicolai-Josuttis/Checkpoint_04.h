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

struct CopyBox2Strings : public StringsBox {

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< std::is_convertible_v<S1, String> >
    > CopyBox2Strings(S1&& first, S2&& last = "")
        : StringsBox( first
                    , last ) {}
};

struct MoveBox2Strings : public StringsBox {

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< std::is_convertible_v<S1, String> >
    > MoveBox2Strings(S1&& first, S2&& last = "")
        : StringsBox( std::is_same< decltype(first), typename std::remove_reference<S1>::type&& >::value ? move(first) : first
                    , std::is_same< decltype(last), typename std::remove_reference<S2>::type&& >::value ? move(last) : last ) {}
};

struct GreedyBox2Strings : public StringsBox {

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< std::is_convertible_v<S1, String> >
    > GreedyBox2Strings(S1&& first, S2&& last = "")
        : StringsBox( move(first)
                    , move(last) ) {}
};

void Checkpoint_04()
{
    checkpoint(04_Begin);
    {
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

        CopyBox2Strings cb1("abc", str1);

        cb1.print();
        std::cout << str1 << '\n';

        CopyBox2Strings cb2("abc", move(str1));
        cb2.print();
        std::cout << str1 << '\n';

        checkpoint(04_B_MoveBox);

        MoveBox2Strings mb1("abc", str1);

        mb1.print();
        std::cout << str1 << '\n';

        MoveBox2Strings mb2("abc", move(str1));
        mb2.print();
        std::cout << str1 << '\n';

        checkpoint(04_B_GreedyBox);

        GreedyBox2Strings gb1("abc", str2);

        gb1.print();
        std::cout << str2 << '\n';

        GreedyBox2Strings gb2("abc", str2);

        gb2.print();
        std::cout << str2 << '\n';


        checkpoint(04_End);
    }
}
