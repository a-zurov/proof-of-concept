#pragma once

#include <vector>
#include <list>

#include "String.h"

using String = cxx::String;

struct SlowBox4StringVector {

    std::vector<String>  m_vecStrings;

    template <class... Args>
    SlowBox4StringVector(Args... args) {

        std::list<const char*> listArgs = { args... };

        m_vecStrings.reserve(listArgs.size());

        while (!listArgs.empty()) {
            m_vecStrings.emplace_back(listArgs.front());
            listArgs.pop_front();
        }
    }
};

void Checkpoint_03()
{
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
}
