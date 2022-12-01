#pragma once

#include <type_traits>

#include "macro.h"

template <typename T>
typename std::remove_reference<T>::type && move(T&& x) {
    cout_dump();
    return static_cast< typename std::remove_reference<T>::type && >(x);
}

void Checkpoint_04()
{
    checkpoint(04_Begin);
    {
        int x = 1;

        int&& rref_x = move(x);
        int& ref_rref_x = rref_x;

        ++rref_x;

        std::cout << x << rref_x << ref_rref_x << '\n';

        int&& rref_rref_x = move(rref_x);
        int& ref_rref_rref_x = rref_rref_x;

        ref_rref_rref_x = 0;

        std::cout << x << rref_x << ref_rref_x << rref_rref_x << ref_rref_rref_x << '\n';
    }
}
