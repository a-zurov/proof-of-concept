//////////////////////////////////////////////////////////////////////////
// Module: ILogHandler.h
//////////////////////////////////////////////////////////////////////////

#ifndef __XCOM_I_LOG_HANDLER_H__
#define __XCOM_I_LOG_HANDLER_H__

//////////////////////////////////////////////////////////////////////////

#include "Types.h"
#include "IBase.h"

//////////////////////////////////////////////////////////////////////////

XCOM_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////
// {71D2411E-4371-4a1d-A015-F3A2A252A08E}

static const XCOM_UUID ILogHandler_UUID =
{ 0x71d2411e, 0x4371, 0x4a1d, { 0xa0, 0x15, 0xf3, 0xa2, 0xa2, 0x52, 0xa0, 0x8e } };

class ILogHandler
{
public:
    virtual ~ILogHandler() {}

    virtual void OnLogMessage(const LogLevel nLogLevel, const char* pchMessage) = 0;
};

//////////////////////////////////////////////////////////////////////////

XCOM_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif //__XCOM_I_LOG_HANDLER_H__
