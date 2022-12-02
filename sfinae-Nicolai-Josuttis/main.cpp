// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// Inspired by CppCon 2017: Nicolai Josuttis “The Nightmare of Move Semantics for Trivial Classes”:
// https://www.youtube.com/watch?v=PNRju6_yn3o
// Presentation Slides, PDFs, Source Code and other presenter materials are available at:
// https://github.com/CppCon/CppCon2017
// ------------------------------------------------------------------------------------------------

#include <iostream>

#include "String.h"

#include "Checkpoint_01.h"
#include "Checkpoint_02.h"
#include "Checkpoint_03.h"
#include "Checkpoint_04.h"

#include "Customer.h"

static constexpr cxx::constString const& g_CxxVersionName(cxx::getCxxVersionName());

using String = cxx::String;

template<typename T>
void f(T&& x) {

    std::cout << (std::is_same< decltype(x), typename std::remove_reference<T>::type&& >::value ? "decltype(x) = T &&" : (
        std::is_same< decltype(x), typename std::remove_reference<T>::type& >::value ? "decltype(x) = T &" : "decltype(x) = T"));

    std::cout << '\n';
}

int main()
{
    std::cout << "Hello Move Semantics with " << g_CxxVersionName << "!\n";

    //Checkpoint_01(); // class String initialization and assignment
    //Checkpoint_02(); // FastBox and SlowBox classes
    //Checkpoint_03(); // std::vector::push_back catastrophe, emplace_back

    Checkpoint_04();

    f(0);
    int x = 0;
    int& ref_x = x;
    f(ref_x);
    int&& rref_x = std::move(x);
    f(rref_x);

    //Checkpoint_0X();
    //Checkpoint_0Z();
}
