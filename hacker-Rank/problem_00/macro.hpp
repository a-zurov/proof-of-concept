#ifndef __MACRO_HPP__
#define __MACRO_HPP__

#include <iostream>
#include <cstring>
#include <chrono>

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

#ifdef ALLOW_FLUSH_ON_DUMP
#define DUMP_ENDL std::endl
#else
#define DUMP_ENDL '\n'
#endif

class DumpDuration
{
    using steady_clock = std::chrono::steady_clock;

public:
    explicit DumpDuration(const std::string& szMessage = "")
        : m_szMessage(szMessage + ": ")
        , m_timeStart(steady_clock::now())
    {
    }

    ~DumpDuration() {
        auto dur = steady_clock::now() - m_timeStart;
        std::cerr << m_szMessage
            << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()
            << " ms" << DUMP_ENDL;
    }
private:
    std::string m_szMessage;
    steady_clock::time_point m_timeStart;
};

#define UNIQ_ID_IMPL(x) _a_local_var_##x
#define UNIQ_ID(x) UNIQ_ID_IMPL(x)

#ifndef ALLOW_MACRO
#define ALLOW_MACRO
#endif

#endif //__MACRO_HPP__
