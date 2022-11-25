// array.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <type_traits>
#include <algorithm>

template< typename T,
        typename = std::enable_if_t< std::is_array<T>::value >
        >
void sort(T* p) {
    std::sort( *p, *(p + 1) );
}

int main()
{
    int a[6] = { 3, 6, 1, 5, 4, 2};

    sort(&a);

    /*
    int(*ptr_a)[5] = &a;
    sort(ptr_a);
    */

    for (int j = 0; j < 6; ++j) {
        std::cout << *(a + j) << ' ';
    }
    std::cout << '\n';

    int* p = a + 4; // it's just a joke:

    std::cout   << a[4]         - a[3]      << " = "
                << *(a + 4)     - *(3 + a)  << " = "
                << *p           - 3[a]      << " = "
                << 0[p - 0]     - 3[p - 4]  << '\n';
}
