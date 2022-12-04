#pragma once

#include <type_traits>

#include "macro.h"
#include "String.h"

template <typename T>
typename std::remove_reference<T>::type && move(T&& arg) noexcept {
    cout_dump_msg(arg);
    return static_cast< typename std::remove_reference<T>::type && >(arg);
}

template <typename T> // forward an lvalue as either an lvalue or an rvalue
constexpr T&& forward(typename std::remove_reference<T>::type& arg) noexcept {
    return static_cast<T&&>(arg);
}

template <typename T> // forward an rvalue as an rvalue
constexpr T&& forward(typename std::remove_reference<T>::type&& arg) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward call");
    return static_cast<T&&>(arg);
}

using String = cxx::String;

struct StringsBox {

    String  m_szFirst;
    String  m_szLast;

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< std::is_convertible_v<S1, String> >
    >StringsBox(S1&& first, S2&& last = "")
        : m_szFirst( forward<S1>(first) )
        , m_szLast( forward<S2>(last) )
    {
        cout_dump();
    }

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
                    , last )
    {
        cout_dump();
        if (std::is_same< decltype(first), typename std::remove_reference<S1>::type&& >::value)
            std::cout << "S1 arg : decltype(arg) = remove_reference<S1>::type&&\n";
        if (std::is_same< decltype(last), typename std::remove_reference<S2>::type&& >::value)
            std::cout << "S2 arg : decltype(arg) = remove_reference<S2>::type&&\n";
    }
};

struct MoveBox : public StringsBox {

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< std::is_convertible_v<S1, String> >
    > MoveBox(S1&& first, S2&& last = "")
        : StringsBox( std::is_same< decltype(first), typename std::remove_reference<S1>::type&& >::value ? move(first) : first
                    , std::is_same< decltype(last), typename std::remove_reference<S2>::type&& >::value ? move(last) : last )
    {
        cout_dump();
        if (std::is_same< decltype(first), typename std::remove_reference<S1>::type&& >::value)
            std::cout << "S1 arg : decltype(arg) = remove_reference<S1>::type&&\n";
        if (std::is_same< decltype(last), typename std::remove_reference<S2>::type&& >::value)
            std::cout << "S2 arg : decltype(arg) = remove_reference<S2>::type&&\n";
    }
};

struct GreedyBox : public StringsBox {

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< std::is_convertible_v<S1, String> >
    > GreedyBox(S1&& first, S2&& last = "")
        : StringsBox( move(first)
                    , move(last) )
    {
        cout_dump();
        if (std::is_same< decltype(first), typename std::remove_reference<S1>::type&& >::value)
            std::cout << "S1 arg : decltype(arg) = remove_reference<S1>::type&&\n";
        if (std::is_same< decltype(last), typename std::remove_reference<S2>::type&& >::value)
            std::cout << "S2 arg : decltype(arg) = remove_reference<S2>::type&&\n";
    }
};


struct PerfectBox : public StringsBox {

    template< typename S1
        , typename S2 = String
        , typename = std::enable_if_t< std::is_convertible_v<S1, String> >
    > PerfectBox(S1&& first, S2&& last = "")
        : StringsBox(forward<S1>(first), forward<S2>(last))
    {
        cout_dump();
    }
};

template <typename T >
class C {
    C() = delete;
};

template<typename T>
void f(T&& arg) {

    //C<T> c; // deduction Meyers' maneuver

    std::cout << ( std::is_same< decltype(arg), typename std::remove_reference<T>::type&& >::value ? "decltype(arg) = T&&" :
                    ( std::is_same< decltype(arg), typename std::remove_reference<T>::type& >::value ? "decltype(arg) = T&" : "decltype(arg) = T" )
                 );

    std::cout << '\n';
}

void Checkpoint_04()
{
    checkpoint(04_Begin);
    {

        f(0);

        const int a = 0;
        f(a);

        int y = 0;
        f(y);

        int& ref_y = y;
        f(ref_y);

        int&& rref_y = move(y);
        f(rref_y);

        String s("xxx");
        f(move(s));

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
        String str3 = "xyz";
        String str4 = str3;

        checkpoint(04_B_CopyBox);

        CopyBox cb1("abc", str1);
        cb1.print();
        std::cout << str1 << '\n';

        CopyBox cb2("abc", move(str1));
        cb2.print();
        std::cout << str1 << '\n';

        CopyBox cb3(str2, move(str3));
        cb3.print();
        std::cout << str2 << ' ' << str3 << '\n';

        checkpoint(04_B_MoveBox);

        MoveBox mb1("abc", str1);
        mb1.print();
        std::cout << str1 << '\n';

        MoveBox mb2("abc", move(str1));
        mb2.print();
        std::cout << str1 << '\n';

        MoveBox mb3(str2, move(str3));
        mb3.print();
        std::cout << str2 << ' ' << str3 << '\n';

        checkpoint(04_B_GreedyBox);

        GreedyBox gb1("abc", str2);
        gb1.print();
        std::cout << str2 << '\n';

        GreedyBox gb2("abc", move(str4));
        gb2.print();
        std::cout << str4 << '\n';

        checkpoint(04_B_PerfectBox);

        str1 = "perfect";
        str2 = "forwarding";

        PerfectBox pb1(str1, str2);
        pb1.print();
        std::cout << str1 << ' ' << str2 << '\n';

        PerfectBox pb2(move(str1), move(str2));
        pb2.print();
        std::cout << str1 << ' ' << str2 << '\n';

        checkpoint(04_C); // you shouldn't code like as below

        String&& rref_str = String("abc");
        String str("cde");
        const String& cref_str = str;

        MoveBox mb(rref_str, move(cref_str));
        mb.print();
        std::cout << str << '\n';

        checkpoint(04_End);
    }
}
