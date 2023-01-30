// init.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------
//

#include <iostream>
#include <vector>

//-------------------------------------------------
// aggregate initialization (see below)

struct Base {
    int j;
};

struct Middle : public Base {
    float t;
};

struct Derived : public Middle {
    char c;
};

//-------------------------------------------------
// The most vexing parse is a counterintuitive form
// of syntactic ambiguity resolution in the C++.

struct list_t {};
struct myclass_t {
    int x = 42;
    myclass_t(list_t, list_t) {}
};

int main() {
    // since C++17
    // aggregate initialization by the initializer list
    Derived d { { {5}, .2f }, 'a' };

    // most vexing parse in direct initialization
    myclass_t m1( (list_t()), (list_t() ) ); // (A) most vexing parse without parentheses
    myclass_t m2{ list_t(), list_t() };

    std::cout << m1.x << '\n'; // CE without additional parentheses in (A)
    std::cout << m2.x << '\n';
};
