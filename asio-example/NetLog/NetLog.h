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
        void Init()
        {
            spNetLogImpl_ = std::make_unique<NetLogImpl>();
        }

        void PingServer()
        {
            spNetLogImpl_->PingServer();
        }

        void FireLogMessage(int line, const char* file, const char* func, const char* pMsg = nullptr)
        {
            spNetLogImpl_->FireLogMessage(line, file, func, pMsg);
        }

        // FOR TESTS ONLY
        void Stop()
        {
            spNetLogImpl_->Stop();
        }
    };

    typedef olc::singleton< INetLog > NetLog;
}
