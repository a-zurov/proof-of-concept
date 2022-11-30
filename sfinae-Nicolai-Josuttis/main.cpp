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

#include "Customer.h"

static constexpr cxx::constString const& g_CxxVersionName(cxx::getCxxVersionName());

using String = cxx::String;



int main()
{
    std::cout << "Hello Move Semantics with " << g_CxxVersionName << "!\n";

    //Checkpoint_01(); // class String initialization and assignment
    //Checkpoint_02(); // FastBox and SlowBox classes
    //Checkpoint_03(); // std::vector::push_back catastrophe, emplace_back


    int x = 1;

    int&& rref_x = std::move(x);
    int& ref_rref5 = rref_x;

    ++rref_x;

    std::cout << x << rref_x << ref_rref5  << '\n';

    int&& rref_rref_x = std::move(rref_x);
    int& ref_rref_rref_x = rref_rref_x;

    ref_rref_rref_x = 0;

    std::cout << x << rref_x << ref_rref5 << rref_rref_x << ref_rref_rref_x << '\n';


    //Checkpoint_0X();
    //Checkpoint_0Z();
}
