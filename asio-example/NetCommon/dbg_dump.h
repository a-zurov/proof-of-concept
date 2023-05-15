#pragma once

#include <iostream>
#include <sstream>
#include <mutex>
#include "singleton.h"

#if defined( _MSC_VER )
#define __CXX_VER__ _MSVC_LANG
#define __PRETTY_FUNCTION__ __FUNCSIG__
#define __DELIM__ '\\'
#else
#define __CXX_VER__ __cplusplus
#define __DELIM__ '/'
#endif

#define __FILENAME__ ( std::strrchr( "/" __FILE__, __DELIM__ ) + 1 )
#define __MAKE_DUMP__ __FILENAME__ << " ( " << __LINE__ << " ) " << __PRETTY_FUNCTION__


namespace olc
{
    class CLog
    {
    public:
        virtual ~CLog() {}

        CLog() {}

        void FireLogMessage(const std::string& msg) {

            std::unique_lock<std::mutex> lock(mtxFireLog_);
            std::cout << msg << '\n';
        }

    private:
        std::mutex mtxFireLog_;
    };

    typedef singleton< CLog > SLog;

    class DbgDump
    {
        std::string file_;
        std::string func_;

        int line_;

    public:
        DbgDump(const char* file, const char* func, int line)
            : file_(file)
            , func_(func)
            , line_(line)
        {
            std::stringstream ss;
            ss << std::this_thread::get_id() << " >> ";
            ss << file_ << " ( " << line_ << " ) " << func_;

            SLog::instance().FireLogMessage(ss.str());
        }

        ~DbgDump()
        {
            std::stringstream ss;
            ss << std::this_thread::get_id() << " << ";
            ss << file_ << " ( " << line_ << " ) " << func_;

            SLog::instance().FireLogMessage(ss.str());
        }
    };
}

#define ALLOW_DBG_DUMP_
#ifdef ALLOW_DBG_DUMP_
#define DBG_DUMP()      olc::DbgDump dbg_dump( __FILENAME__, __PRETTY_FUNCTION__, __LINE__ )
#define DBG_MSG(x)      olc::SLog::instance().FireLogMessage(x)
#define DBG_MSG_EX(x)   std::stringstream ss__; ss__ << x; olc::SLog::instance().FireLogMessage(ss__.str())
#define DBG_MSG_EX2(y, x)   std::stringstream ss__;                     \
                        ss__ << std::this_thread::get_id() << " | ";    \
                        ss__ << __MAKE_DUMP__ << "\n " << y << " " << x;\
                        olc::SLog::instance().FireLogMessage(ss__.str())
#define DBG_MSG_SRV(x)  DBG_MSG_EX2("[SERVER]", x)
#define DBG_MSG_CLT(x)  DBG_MSG_EX2("[CLIENT]", x)
#define DBG_MSG_CNN(y, x)  DBG_MSG_EX2("[CONNECT " << y << "] ", x)
#else
#define DBG_DUMP()
#define DBG_MSG(x)
#define DBG_MSG_EX(x)
#define DBG_MSG_SRV(x)
#define DBG_MSG_CLT(x)
#define DBG_MSG_CNN(x)
#endif
