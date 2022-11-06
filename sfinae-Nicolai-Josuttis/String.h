#pragma once

#include "macro.h"

BEGIN_NAMESPACE_CCN

class String {

    char*   m_pBuff;
    size_t  m_nSize;

public: // The Rule of Three

    ~String();

    String();
    String(const char* p);
    String(const String& s);

    String& operator= (const String& rhs);

public: // The Rule of Five

    String(String&& rhs) noexcept;

    String& operator= (String&& rhs) noexcept;

public:

    char operator[] (size_t j) const
    {
        if (!m_pBuff) throw std::invalid_argument("");

        return j < m_nSize ? m_pBuff[j] : throw std::out_of_range("");
    }

    friend std::ostream& operator << (std::ostream& os, const String& s);
};

std::ostream& operator << (std::ostream& os, const String& s);

END_NAMESPACE_CCN
