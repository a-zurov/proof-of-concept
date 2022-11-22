#pragma once

#include "macro.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_NAMESPACE_CXX

//////////////////////////////////////////////////////////////////////////

class String {

    char*   m_pBuff;
    size_t  m_nSize;

public: // The Rule of Three

    ~String();

    String();                   // A
    String(const char* p);      // B
    String(const String& s);    // C

    String& operator= (const String& rhs);      // D

public: // The Rule of Five

    String(String&& rhs) noexcept;              // E

    String& operator= (String&& rhs) noexcept;  // F

public:

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
