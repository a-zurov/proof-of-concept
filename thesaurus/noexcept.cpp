// noexcept.cpp : This file contains the 'main' function. Program execution begins and ends there.
// g++ -std=c++14 -c noexcept.cpp
// ------------------------------------------
// throw
// new
// dynamic_cast
// function call without noexcept

#include <iostream>

void may_throw();
void no_throw() noexcept;
auto may_throw_lambda = [] {};
auto no_throw_lambda = []() noexcept {};

struct Div
{
    static int f(int x, int y) // noexcept(false)
    {
        if (0 == y) throw 1;
        return x / y;
    }
};

struct Mult
{
    static int f(int x, int y) noexcept
    {
        return x * y;
    }
};

template <typename T>
int g(int x, int y) noexcept(noexcept(T::f(x, y)))
{
    return T::f(x, y);
}

int main()
{
    std::cout << std::boolalpha
        << "Is g<Div>(1, 0) noexcept? " << noexcept(g<Div>(1, 0)) << '\n'
        << "Is g<Mult>(1, 0) noexcept? " << noexcept(g<Mult>(1, 0)) << '\n'
        << "Is may_throw() noexcept? " << noexcept(may_throw()) << '\n'
        << "Is no_throw() noexcept? " << noexcept(no_throw()) << '\n'
        << "Is may_throw_lambda() noexcept? " << noexcept(may_throw_lambda()) << '\n'
        << "Is no_throw_lambda() noexcept? " << noexcept(no_throw_lambda()) << '\n';
}
