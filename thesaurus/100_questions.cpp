// 100_questions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#if 1
// Q3. Which of the following operator can be overloaded through friend function ->  =  ()  *

struct S {
    int x_;
};

struct CS_Ptr
{
    S* pS_;

    CS_Ptr(S* p = nullptr) : pS_(p) {}

    friend S* operator*(const CS_Ptr& sp);
};

S* operator* (const CS_Ptr& sp) {
    return sp.pS_;
};

int main() {

    CS_Ptr sp{ new S{5} };
    std::cout << (*sp)->x_;
    delete *sp;
}
// A1. Operators that cannot be overloaded are  .*  ::  ?:
// Operators that cannot be overloaded when declaring that function as friend function are  =  ()  []  ->
#endif
