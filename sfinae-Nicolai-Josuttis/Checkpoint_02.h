#pragma once

#include "String.h"

using String = cxx::String;

struct FastBox {
    String  m_szBoxed;
    FastBox(const char* p)
        : m_szBoxed(p) {
    }
};

struct SlowBox {
    String  m_szBoxed;
    SlowBox(const char* p) {
        m_szBoxed = p;
    }
};

void Checkpoint_02()
{
    checkpoint(02_Begin);
    {
        checkpoint(02_A);

        FastBox fb1("abc"); // C1, the same: FastBox fb1 = "abc";
        FastBox fb2(fb1);   // CC, the same: FastBox fb2 = fb1;

        checkpoint(02_B);

        SlowBox sb1("xyz"); // C0, C1, MASS, D (tmp_String)
        SlowBox sb2(sb1);   // CC, the same: SlowBox sb2 = sb1;

        checkpoint(02_End);
    }
}
