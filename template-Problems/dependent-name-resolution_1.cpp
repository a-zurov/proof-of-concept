//----------------------------------------------------------------------------
// The Dependent Name Resolution compiler problem for Templates
//----------------------------------------------------------------------------
// 'typename' and 'class' are interchangeable in the basic case of specifying
// a template parameters:
// template<class T> class Foo {};
// template<typename T> class Foo {};
// But.. 'typename' is used inside a template body to declare when you are referencing
// a dependent name as type that depends on template parameter.
// https://en.cppreference.com/w/cpp/language/dependent_name
//----------------------------------------------------------------------------

template < typename T>
struct S {
    using X = T;
};

template <>
struct S<int> {
    static const int X = 2;
};

int a = 2;

template <typename T>
void f() {

    typename S<T>::X* a;
}

int main()
{
    // f<int>();
    f<double>();
}
