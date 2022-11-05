// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// Inspired by CppCon 2017: Nicolai Josuttis “The Nightmare of Move Semantics for Trivial Classes”:
// https://www.youtube.com/watch?v=PNRju6_yn3o
// Presentation Slides, PDFs, Source Code and other presenter materials are available at:
// https://github.com/CppCon/CppCon2017
// ------------------------------------------------------------------------------------------------

#include <iostream>
#include "macro.h"

static constexpr ccn::conststr const& g_CxxVersionName(ccn::getCxxVersionName());

int main()
{
    std::cout << "Hello Move Semantics with " << g_CxxVersionName << "!\n";

    checkpoint(0);
    cout_dump();
    cout_dump_msg("Ish govna kakya!!");
}
