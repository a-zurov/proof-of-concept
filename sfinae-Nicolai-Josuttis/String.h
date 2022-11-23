#pragma once

#include "macro.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_NAMESPACE_CXX

//////////////////////////////////////////////////////////////////////////

class String {

    char*   m_pBuff;
    size_t  m_nSize;

public: // The Rule of Three

    ~String();                  // D (dtor)

    String();                   // C0 (ctor)
    String(const char* p);      // C1 (cnvctor)
    String(const String& s);    // CC (cpyctor)

    String& operator= (const String& rhs);      // ASS (assop)

public: // The Rule of Five

    String(String&& rhs) noexcept;              // MC (movctor)

    String& operator= (String&& rhs) noexcept;  // MASS (movassop)

public: // Extra assignment
/*
    String& operator= (const char* p); // EXTASS
*/

    char operator[] (size_t j) const
    {
        if (!m_pBuff) throw std::invalid_argument("");

        return j < m_nSize ? m_pBuff[j] : throw std::out_of_range("");
    }

    friend std::ostream& operator << (std::ostream& os, const String& s);
};

std::ostream& operator << (std::ostream& os, const String& s);

//////////////////////////////////////////////////////////////////////////

END_NAMESPACE_CXX

//////////////////////////////////////////////////////////////////////////
