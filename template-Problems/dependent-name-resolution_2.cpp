//----------------------------------------------------------------------------
// The Dependent Name Resolution compiler problem for Templates
//----------------------------------------------------------------------------
// https://learn.microsoft.com/en-us/cpp/cpp/name-resolution-for-dependent-types?view=msvc-170
//----------------------------------------------------------------------------

template <class T> class X
{
public:
    void f(typename T::S*) {}
};

class Y
{
public:
    struct S {};
};

int main()
{
    X<Y> x;
    x.f(new Y::S());
}
