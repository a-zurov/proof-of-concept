#pragma once

#include <memory>
#include "NetLogImpl.h"
#include "./../NetCommon/singleton.h"

namespace olc2
{

    int Add(int x, int y);

    class INetLog
    {
        std::unique_ptr< NetLogImpl> spNetLogImpl_;

    public:
        void Init();

        void PingServer();

        void FireLogMessage(int line, const char* file, const char* func, const char* pMsg = nullptr);

        // FOR TESTS ONLY
        void Stop();
    };

    typedef olc::singleton< INetLog > NetLog;
}

#define ALLOW_NET_DUMP_
#ifdef ALLOW_NET_DUMP_
#define NET_DUMP()      olc2::NetLog::instance().FireLogMessage(__LINE__, __FILENAME__, __PRETTY_FUNCTION__)
#define NET_MSG(x)      std::stringstream ss2__; ss2__ << x; olc2::NetLog::instance().FireLogMessage(__LINE__, __FILENAME__, __PRETTY_FUNCTION__, ss2__.str().c_str())
#else
#define NET_DUMP()
#define NET_MSG(x)
#endif
