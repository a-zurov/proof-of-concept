//----------------------------------------------------------------------------
// The Scott Meyers' maneuver for template arguments deduction
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

template<typename T>
class C {
    C() = delete;
};

template<typename T>
void f(T x) {
    C<T> c;
}

int main()
{
    int x = 0;
    int& y = x;
    const int& z = y;

    f(y);
}
