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

template <>
void sort( int(* p)[6])
{
    std::sort(  static_cast<int*>(*p),
                static_cast<int*>(*(p + 1))
            );
    std::cout << "Yep, I'm here!" << '\n';
}

//--------------------------------------------------------------------------

template <typename T>
std::remove_reference<T>&& underdone_move(T& x) {
    return static_cast< std::remove_reference_t<T>&& >(x);
}

template <typename T>
typename std::decay<T>::type move_like_transform_array_t_to_element_ptr(T* p) {
    return static_cast<std::decay<T>::type>(*p);
}

template< typename T,
             typename = std::enable_if_t< std::is_array<T>::value >
        >
void sort_2(T* p) {
    std::sort(  move_like_transform_array_t_to_element_ptr(p),
                move_like_transform_array_t_to_element_ptr(p + 1),
        []( typename std::remove_pointer_t< typename std::decay<T>::type > a,
            typename std::remove_pointer_t< typename std::decay<T>::type > b)
        {
            return a > b;
        }
    );
}


int main()
{
    int a[6] = { 3, 6, 1, 5, 4, 2};

    sort(&a);

    sort_2(&a);

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
