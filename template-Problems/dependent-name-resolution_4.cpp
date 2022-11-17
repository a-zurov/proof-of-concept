//----------------------------------------------------------------------------
// The Dependent Name Resolution compiler problem for Templates
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

#include <iostream>
#include <type_traits>

template <class T>
struct S {
    template <int M, int N>
    struct X {
        int constexpr g() {
            return M + N;
        }

        template <int m, int n>
        T constexpr h(typename S<T>::template X<m, n> x) {
            return N + n, M + m;
        }
    };
};

template <>
struct S<int> {
    static const int X = 1;
};

int a = 1;

template <typename T>
void f() {
    typename S<T>::template X<1, 2> a;
    if (std::is_arithmetic<T>::value && !std::is_same<T, bool>::value) {
        typename S<T>::template X<1, a.g()> b;
        std::cout << "The Meaning of life = " << b.g() << a.h(b) << std::endl;
    } else {
        std::cout << "..but sometimes life has no meaning." << std::endl;
    }
};

int main()
{
    //f<int>();
    f<double>();
    f<bool>();
}
