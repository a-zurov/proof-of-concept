#pragma once

#include "String.h"

using String = cxx::String;

void Checkpoint_01()
{
    checkpoint(01_Begin);
    {
        checkpoint(01_A);

        String szFirst = "abc";                 // C1
        String szSecond("xyz");                 // C1
        String szThird = szSecond;              // CC
        String szFourth(szFirst);               // CC
        String szFifth(std::move(szThird));     // MCC

        auto kek_print = [&]() {
             std::cout << szFirst
                << " " << szSecond
                << " " << szThird
                << " " << szFourth
                << " " << szFifth << "\n";
        };

        kek_print();

        checkpoint(01_B);

        szFirst = szThird = szSecond;   // ASS <- ASS
        szThird = "gde";                // C1, MASS, D (tmp_String)
        szFourth = std::move(szFifth) = szThird; // no move

        kek_print();

        checkpoint(01_C);

        szFirst = std::move(szSecond);  // MASS
        szSecond = std::forward<String>(std::move(szThird));
        szFourth = szFifth = std::move(szThird); // ASS <- MASS

        kek_print();

        checkpoint(01_End);
    }
}
