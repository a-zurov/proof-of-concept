//----------------------------------------------------------------------------
// The Dependent Name Resolution compiler problem for Templates
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

template <class T>
struct S {
    template <int M, int N>
    struct X {};
};

template <>
struct S<int> {
    static const int X = 1;
};

int a = 1;

template <typename T>
void f() {
    typename S<T>::template X<1, 2> a;
};

int main()
{
    // f<int>();
    f<double>();
}
