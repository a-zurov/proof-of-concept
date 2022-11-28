// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// Inspired by CppCon 2017: Nicolai Josuttis “The Nightmare of Move Semantics for Trivial Classes”:
// https://www.youtube.com/watch?v=PNRju6_yn3o
// Presentation Slides, PDFs, Source Code and other presenter materials are available at:
// https://github.com/CppCon/CppCon2017
// ------------------------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <list>

#include "String.h"

#include "Checkpoint_01.h"
#include "Checkpoint_02.h"

#include "Customer.h"

static constexpr cxx::constString const& g_CxxVersionName(cxx::getCxxVersionName());

using String = cxx::String;

struct SlowBox4StringVector {

    std::vector<String>  m_vecStrings;

    template <class... Args>
    SlowBox4StringVector( Args... args) {

        std::list<const char*> vecArgs = { args... };

        m_vecStrings.reserve( vecArgs.size() );

        while ( !vecArgs.empty() ) {
            m_vecStrings.emplace_back( vecArgs.front() );
            vecArgs.pop_front();
        }
    }
};

int main()
{
    std::cout << "Hello Move Semantics with " << g_CxxVersionName << "!\n";

    //Checkpoint_01(); // class String initialization and assignment
    //Checkpoint_02(); // FastBox and SlowBox classes

    checkpoint(03_Begin);
    {
        std::vector<String> vecStrings;
        vecStrings.reserve(2);

        checkpoint(03_A);

        for (size_t j = 0; j < 3; ++j) {
            cout_dump_msg("03_A_" << j);
            vecStrings.push_back(String("abc"));
        }

        checkpoint(03_B);

        SlowBox4StringVector sbsv("abc", "cde", "xyz");

        checkpoint(03_End);
    }

    //Checkpoint_0X();
    //Checkpoint_0Z();
}
