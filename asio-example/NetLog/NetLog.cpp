// NetLog.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "NetLog.h"

namespace olc2
{
    // TODO: This is an example of a library function
    int Add(int x, int y)
    {
        return x + y;
    }

    void INetLog::Init()
    {
        spNetLogImpl_ = std::make_unique<NetLogImpl>();
    }

    void INetLog::PingServer()
    {
        spNetLogImpl_->PingServer();
    }

    void INetLog::FireLogMessage(int line, const char* file, const char* func, const char* pMsg)
    {
        spNetLogImpl_->FireLogMessage(line, file, func, pMsg);
    }

    // FOR TESTS ONLY
    void INetLog::Stop()
    {
        spNetLogImpl_->Stop();
    }
}
