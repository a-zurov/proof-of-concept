//----------------------------------------------------------------------------
// The postfix increment semantics r-value and operator precedence problem
// (with an example template friends declaration)
//----------------------------------------------------------------------------
// https://en.cppreference.com/w/cpp/language/operator_precedence
//----------------------------------------------------------------------------

#include <iostream>

template <typename T>
class X {
    T m_x;
public:
    X(T t) : m_x(t) {}

    X<T>& operator++ ();
    X<T> operator++ (int);
    X<T>& operator+= (const X<T>&);

    template <typename T>
    friend std::ostream& operator << (std::ostream&, const X<T>&);
};

// Define prefix increment operator
template <typename T>
X<T>& X<T>::operator++ () {
    X<T>::m_x++;
    return *this;
}

// Define postfix increment operator.
template <typename T>
X<T> X<T>::operator++ (int) {
    X<T> tmp = *this;
    ++*this;
    return tmp;
}

template <typename T>
X<T>& X<T>::operator+= (const X<T>& rhs) {
    X<T>::m_x += rhs.m_x;
    return *this;
}

template <typename T>
std::ostream& operator << (std::ostream& os, const X<T>& x) {
    return os << x.m_x;
}


int main() {

    int j = 0;
    (++j)++;
    ++++j;
    ++j = std::move(j++);
    j += std::forward<int>(std::move(j++));
    std::cout << j; // CE, UB, result?

    X<double> x(0);
    (++x)++;
    ++++x;
    ++x = std::move(x++);
    x += std::move(x++);
    std::cout << x;
}
