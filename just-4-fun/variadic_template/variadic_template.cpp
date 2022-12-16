// variadic_template.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

static int counter = 0;

template <bool arg1>
int suck_variadic_arg()
{
    std::cout << counter++ << ": suck " << arg1 << '\n';
    return arg1;
}
template <bool arg1, bool arg2, bool... args>
int suck_variadic_arg()
{
    std::cout << counter++ << ": suck " << arg1 << '\n';
    return (suck_variadic_arg<arg2, args...>() << 1) + arg1;
}

template <bool... args>
static int reverse()
{
    std::cout << "pumping complete/reverse started" << '\n';
    return suck_variadic_arg< args...>();
};

template <int Number, int Digits, bool arg1, bool... args>
struct Plunger;

template <int Number, int Digits, bool... args>
struct Multiplexer
{
    static int prepare_variadic_arg()
    {
        std::cout << Digits << ": pump ";
        std::cout << (bool)((1 << Digits) & Number) << '\n';
        return Plunger< Number, Digits - 1, (bool)((1 << Digits)& Number), args...>::pump_variadic_arg();
    }
};

template <int Number, int Digits, bool arg1, bool... args>
struct Plunger
{
    static int pump_variadic_arg()
    {
        return Multiplexer< Number, Digits, arg1, args...>::prepare_variadic_arg();
    }
};

template <int Number, bool arg1, bool... args>
struct Plunger< Number, 0, arg1, args...>
{
    static int pump_variadic_arg()
    {
        std::cout << 0 << ": pump ";
        std::cout << (bool)(1 & Number) << '\n';
        return reverse< (bool)(1 & Number), arg1, args...>();
    }
};

template <int Number, int Digits>
struct VariadicArgsPump
{
    static int start()
    {
        return Multiplexer< Number, Digits>::prepare_variadic_arg();
    }
};

int main()
{
    std::cout << VariadicArgsPump< 17, 7>::start();
}
