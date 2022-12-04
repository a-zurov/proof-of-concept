#include "String.h"

#define _ALLOW_STRING_BUFF_DUMP

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
    if (nullptr == p) {

        m_nSize = 0;
        m_pBuff = nullptr;
        cout_dump_this();
        return;
    }

    m_nSize = strlen(p);
    m_pBuff = new char[m_nSize + 1];

    memcpy(m_pBuff, p, m_nSize * sizeof(char));
    m_pBuff[m_nSize] = '\n';

#ifdef _ALLOW_STRING_BUFF_DUMP
    cout_dump_this_msg(m_nSize, m_pBuff);
#else
    cout_dump_this();
#endif
}

String::String(const String& s)
    : m_nSize(s.m_nSize)
{

    if (nullptr == s.m_pBuff) {
        m_pBuff = nullptr;
        cout_dump_this();
        return;
    }

    m_pBuff = new char[m_nSize + 1];
    memcpy(m_pBuff, s.m_pBuff, m_nSize + 1);

#ifdef _ALLOW_STRING_BUFF_DUMP
    cout_dump_this_msg(m_nSize, m_pBuff);
#else
    cout_dump_this();
#endif
}

String& String::operator= (const String& rhs) // assignment operator
{


    if (this == &rhs) {
        cout_dump_this();
        return *this;
    }

    delete[] m_pBuff;
    m_nSize = rhs.m_nSize;

    if (nullptr == rhs.m_pBuff) {
        m_pBuff = nullptr;
        cout_dump_this();
        return *this;
    }

    m_pBuff = new char[m_nSize + 1];
    memcpy(m_pBuff, rhs.m_pBuff, m_nSize + 1);

#ifdef _ALLOW_STRING_BUFF_DUMP
    cout_dump_this_msg(m_nSize, m_pBuff);
#else
    cout_dump_this();
#endif

    return *this;
}

//-------------------------------------------------
// The Rule of Five (move semantics)
//-------------------------------------------------

String::String(String&& rhs) noexcept
    : m_nSize(rhs.m_nSize)
    , m_pBuff(rhs.m_pBuff)
{
#ifdef _ALLOW_STRING_BUFF_DUMP
    cout_dump_this_msg(m_nSize, m_pBuff);
#else
    cout_dump_this();
#endif

    rhs.m_nSize = 0;
    rhs.m_pBuff = nullptr;
}

String& String::operator= (String&& rhs) noexcept
{
    if (this == &rhs) {
        cout_dump_this();
        return *this;
    }

    delete[] m_pBuff;

    m_nSize = rhs.m_nSize;
    m_pBuff = rhs.m_pBuff;

    rhs.m_nSize = 0;
    rhs.m_pBuff = nullptr;

#ifdef _ALLOW_STRING_BUFF_DUMP
    cout_dump_this_msg(m_nSize, m_pBuff);
#else
    cout_dump_this();
#endif

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
