#ifndef __MACRO_H__
#define __MACRO_H__

#include <iostream>
#include <cstring>
#include <stdexcept>

#ifdef _MSC_VER
#define __CXX_VER__ _MSVC_LANG
#define __PRETTY_FUNCTION__ __FUNCSIG__
#define __DELIM__ '\\'
#else
#define __CXX_VER__ __cplusplus
#define __DELIM__ '/'
#endif

// C++11 requires a space between literal and string macro
#define __FILENAME__ ( std::strrchr( "/" __FILE__, __DELIM__ ) + 1 )
#define __MAKE_DUMP__   __FILENAME__ << " : " << __LINE__ << " | " << __PRETTY_FUNCTION__

#define cout_dump()  std::cout << __MAKE_DUMP__ << std::endl
#define cout_dump_msg(x)  std::cout << __MAKE_DUMP__ << " " << x << std::endl
#define checkpoint(x) std::cout << __MAKE_DUMP__ << " Checkpoint#" << #x << std::endl


#define BEGIN_NAMESPACE_CCN namespace ccn { // begin check concept namespace
#define END_NAMESPACE_CCN }                 // end check concept namespace

//--------------------------------------------------------------------------------

BEGIN_NAMESPACE_CCN

class conststr
{
    const char* m_pBuff;
    std::size_t m_nSize;

public:
    template<std::size_t N>
    constexpr conststr(const char(&a)[N])
                : m_pBuff(a)
                , m_nSize(N - 1) {}

    // constexpr functions signal errors by throwing exceptions
    // in C++11, they must do so from the conditional operator ?:
    constexpr char operator[](std::size_t n) const
    {
        return n < m_nSize ? m_pBuff[n] : throw std::out_of_range("");
    }

    constexpr std::size_t size() const { return m_nSize; }

    friend std::ostream& operator << (std::ostream& os, const conststr& s);
};

std::ostream& operator << (std::ostream& os, const conststr& s) {

    return s.m_pBuff ? os.write(s.m_pBuff, s.m_nSize) : os << "\"nullptr\"";
}

/**
* The macro BOOST_CXX_VERSION is set to the C++ standard version -
* it has the same value as __cplusplus * if __cplusplus is set to a sensible value,
* otherwise the current C++ standard level in effect
* (for example for MSVC this is set to the value of _MSVC_LANG).
* https://www.boost.org/doc/libs/master/libs/config/doc/html/boost_config/boost_macro_reference.html
*/

#define _Cxx_98 199711L
#define _Cxx_11 201103L
#define _Cxx_14 201402L
#define _Cxx_17 201703L
#define _Cxx_20 202002L

#define _NAME(x) #x

constexpr conststr getCxxVersionName()
{
    switch (__CXX_VER__)
    {
    case _Cxx_98: return _NAME(_Cxx_98);
    case _Cxx_11: return _NAME(_Cxx_11);
    case _Cxx_14: return _NAME(_Cxx_14);
    case _Cxx_17: return _NAME(_Cxx_17);
    case _Cxx_20: return _NAME(_Cxx_20);
    default:
        return "unknown Cxx version";
    }
}

END_NAMESPACE_CCN

#endif // __MACRO_H__
