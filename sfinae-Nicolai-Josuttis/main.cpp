// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// Inspired by CppCon 2017: Nicolai Josuttis “The Nightmare of Move Semantics for Trivial Classes”:
// https://www.youtube.com/watch?v=PNRju6_yn3o
// Presentation Slides, PDFs, Source Code and other presenter materials are available at:
// https://github.com/CppCon/CppCon2017
// ------------------------------------------------------------------------------------------------

#include <iostream>
#include "macro.h"
#include "String.h"

static constexpr ccn::conststr const& g_CxxVersionName(ccn::getCxxVersionName());

using String = ccn::String;

int main()
{
    std::cout << "Hello Move Semantics with " << g_CxxVersionName << "!\n";

    checkpoint(0);
    cout_dump();
    cout_dump_msg("Ish govna kakya!!");

    checkpoint(1);
    String szFirst;
    String szSecond("xyz");
    std::cout << szFirst << " " << szSecond << std::endl;
    szFirst = szSecond;
    std::cout << szFirst << " " << szSecond << std::endl;
    szFirst = std::move(szSecond);
    std::cout << szFirst << " " << szSecond << std::endl;

}
