//----------------------------------------------------------------------------
// The interference of specialization and overload for template functions
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

#include <iostream>

template <typename U, typename V>
void f(U, V) {
    std::cout << 1 << " ";
}

template <typename T>
void f(T, T) {
    std::cout << 2 << " ";
}

template <>
void f(int, int) {
    std::cout << 3 << " ";
}

//--------------------------------------

template <typename U, typename V>
void g(U, V) {
    std::cout << 1 << " ";
}

template <>
void g(int, int) {
    std::cout << 3 << " ";
}

template <typename T>
void g(T, T) {
    std::cout << 2 << " ";
}

//---------------------------------

template <typename U, typename V>
void h(U u, V v) {
    f<U, V>(u, v);
}

template <>
void h(int n, int m) {
    f(n, m);
}

template <typename T>
void h(T t1, T t2) {
    f<T, T>(t1, t2);
}

int main() {

    f(0, 0);
    std::cout << std::endl;
    g(0, 0);
    std::cout << std::endl;
    h(0, 0);
}
