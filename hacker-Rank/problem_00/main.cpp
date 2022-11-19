// problem_00 : main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//---------------------------------------------------------------------------------------------------------

#include "macro.h"

#include <iostream>

int main()
{
    cout_dump_msg("start");

    {
        dump_duration("cycle");
        for (int j = 0; j < 1000; j++) {
            std::cout << '.';
        }
    }

    cout_dump_msg("stop");

}
