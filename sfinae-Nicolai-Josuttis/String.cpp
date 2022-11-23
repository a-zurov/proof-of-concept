#include "String.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_NAMESPACE_CXX

//////////////////////////////////////////////////////////////////////////

//-------------------------------------------------
// The Rule of Three
//-------------------------------------------------

String::~String()
{
    cout_dump_this();

    delete[] m_pBuff;
}

String::String()
    : m_pBuff(nullptr)
    , m_nSize(0)
{
    cout_dump_this();
}

String::String(const char* p)
{
    cout_dump_this();

    if (nullptr == p) {

        m_nSize = 0;
        m_pBuff = nullptr;
        return;
    }

    m_nSize = strlen(p);
    m_pBuff = new char[m_nSize + 1];

    memcpy(m_pBuff, p, m_nSize * sizeof(char));
    m_pBuff[m_nSize] = '\n';
}

String::String(const String& s)
    : m_nSize(s.m_nSize)
{
    cout_dump_this();

    if (nullptr == s.m_pBuff) {
        m_pBuff = nullptr;
        return;
    }

    m_pBuff = new char[m_nSize + 1];
    memcpy(m_pBuff, s.m_pBuff, m_nSize + 1);
}

String& String::operator= (const String& rhs) // assignment operator
{
    cout_dump_this();

    if (this == &rhs) {
        return *this;
    }

    delete[] m_pBuff;
    m_nSize = rhs.m_nSize;

    if (nullptr == rhs.m_pBuff) {
        m_pBuff = nullptr;
        return *this;
    }

    m_pBuff = new char[m_nSize + 1];
    memcpy(m_pBuff, rhs.m_pBuff, m_nSize + 1);
    return *this;
}

//-------------------------------------------------
// The Rule of Five (move semantics)
//-------------------------------------------------

String::String(String&& rhs) noexcept
    : m_nSize(rhs.m_nSize)
    , m_pBuff(rhs.m_pBuff)
{
    cout_dump_this();

    rhs.m_nSize = 0;
    rhs.m_pBuff = nullptr;
}

String& String::operator= (String&& rhs) noexcept
{
    cout_dump_this();

    if (this == &rhs) {
        return *this;
    }

    delete[] m_pBuff;

    m_nSize = rhs.m_nSize;
    m_pBuff = rhs.m_pBuff;

    rhs.m_nSize = 0;
    rhs.m_pBuff = nullptr;

    return *this;
}

//-------------------------------------------------
// Extra assignment
//-------------------------------------------------
/*
String& String::operator= (const char* p)
{
    cout_dump_this();

    if (this->m_pBuff == p) {
        return *this;
    }

    delete[] m_pBuff;

    if (nullptr == p) {

        m_nSize = 0;
        m_pBuff = nullptr;
        return *this;
    }

    m_nSize = strlen(p);
    m_pBuff = new char[m_nSize + 1];

    memcpy(m_pBuff, p, m_nSize * sizeof(char));
    m_pBuff[m_nSize] = '\n';

    return *this;
}
*/

std::ostream& operator << (std::ostream& os, const String& s) {

    return s.m_pBuff ? os.write(s.m_pBuff, s.m_nSize) : os << std::quoted("nullptr");

}

//////////////////////////////////////////////////////////////////////////

END_NAMESPACE_CXX

//////////////////////////////////////////////////////////////////////////
