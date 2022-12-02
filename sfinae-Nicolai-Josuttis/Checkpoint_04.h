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

struct GreedyBox2Strings {

    String  m_szFirst;
    String  m_szLast;

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< std::is_convertible_v<S1, String> >
    > GreedyBox2Strings(S1&& first, S2&& last = "")
        : m_szFirst( std::is_same< decltype(first), S1&& >::value ? move(first) : first  )
        , m_szLast( std::is_same< decltype(last), S2&& >::value ? move(last) : last ) {}
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

        checkpoint(04_B_0);

        String str = "cde";

        checkpoint(04_B_1);

        GreedyBox2Strings cbsv("abc", str);

        checkpoint(04_B_2);

        GreedyBox2Strings mbsv("abc", move(str));

        checkpoint(04_End);
    }
}
