//////////////////////////////////////////////////////////////////////////
// Module: ILogHandler.h
//////////////////////////////////////////////////////////////////////////

#ifndef __GCN_I_LOG_HANDLER_H__
#define __GCN_I_LOG_HANDLER_H__

//////////////////////////////////////////////////////////////////////////

#include "Types.h"
#include "IBase.h"

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_BEGIN

/////////////////////////////////////////////////////////////////////
// {920B6877-E906-4C39-8C4C-AF425F0D0BD6}

static const GCN_UUID ILogHandlerSingltone_UUID =
{ 0x920b6877, 0xe906, 0x4c39, { 0x8c, 0x4c, 0xaf, 0x42, 0x5f, 0xd, 0xb, 0xd6 } };

/////////////////////////////////////////////////////////////////////
// {1D07E1D2-99FD-45da-B12B-64D04C5C5BAF}

static const GCN_UUID ILogHandler_UUID =
{ 0x1d07e1d2, 0x99fd, 0x45da, { 0xb1, 0x2b, 0x64, 0xd0, 0x4c, 0x5c, 0x5b, 0xaf } };

class ILogHandler
    : public IBase
{
public:
    virtual ~ILogHandler() {}

    virtual void SetLogLevel(const LogLevel nLogLevel) = 0;
};



class ILogger
    : public IBase
{
public:
    virtual ~ILogger() {}
};

ILogger* GetLogger();

//////////////////////////////////////////////////////////////////////////
// {71D2411E-4371-4a1d-A015-F3A2A252A08E}

static const GCN_UUID ILogHandlerEvents_UUID =
{ 0x71d2411e, 0x4371, 0x4a1d, { 0xa0, 0x15, 0xf3, 0xa2, 0xa2, 0x52, 0xa0, 0x8e } };

class ILogHandlerEvents
{
public:
    virtual ~ILogHandlerEvents() {}

    virtual void OnLogMessage(const LogLevel nLogLevel, const char* szMessage) = 0;
};

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif //__GCN_I_LOG_HANDLER_H__
