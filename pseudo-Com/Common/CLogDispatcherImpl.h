//////////////////////////////////////////////////////////////////////////
// Module: CLogDispatcherImpl - pseudo COM's logs support system
//////////////////////////////////////////////////////////////////////////

#ifndef __XCOM_LOG_DISPATCHER_IMPL_H__
#define __XCOM_LOG_DISPATCHER_IMPL_H__

//////////////////////////////////////////////////////////////////////////

#include <set>

#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "AtomicSync.h"
#include "CRefCounter.h"
#include "CSmartPtr.h"
#include "Singleton.h"

#include "ILog.h"
#include "ILogHandler.h"
#include "IConnectionPointContainer.h"

//////////////////////////////////////////////////////////////////////////

XCOM_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////

class CLogDispatcherImpl
    : public ILogDispatcher
    , protected IConnectionPointContainer
    , protected CRefCounter
{
    typedef std::set< ILogHandler* > LogHandlersSet_t;

public:
    virtual ~CLogDispatcherImpl();

    CLogDispatcherImpl();

    virtual void SetLogLevel(const LogLevel nLogLevel);

    virtual long AddRef();

    virtual long Release();

    LogLevel GetLogLevel();

    void FireLogMessage(const LogLevel nLogLevel, const std::string& sMessage);

protected:
    virtual ResultCode QueryInterface(const XCOM_UUID& uuid, void** ppInterface);

    virtual ResultCode Bind(const XCOM_UUID& uuid, void* pInterface);

    virtual ResultCode Unbind(const XCOM_UUID& uuid, void* pInterface);

    virtual void Close();

    void AddLogHandler(ILogHandler* pEvents);

    void RemoveLogHandler(ILogHandler* pEvents);

private:
    volatile UINT32_t m_nLogLevel = LL_PREBIND;
    LogHandlersSet_t m_setLogHandlers;
    boost::shared_mutex m_mtxLogHanlersGuard;
};

typedef singleton< CLogDispatcherImpl > LogDispatcherSingleton;

typedef CSmartPtr< CLogDispatcherImpl > LogDispatcherPtr_t;

class CLogImpl
    : public ILog
    , protected CRefCounter
{
public:
    CLogImpl();

    virtual ~CLogImpl();

    virtual long AddRef();

    virtual long Release();

    virtual void Close();

    virtual ResultCode QueryInterface(const XCOM_UUID& uuid, void** ppInterface);

private:
    LogDispatcherPtr_t m_spLogDispatcher;
};

//////////////////////////////////////////////////////////////////////////

#include "DumpFunction.hpp"

//////////////////////////////////////////////////////////////////////////

XCOM_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif //__XCOM_LOG_DISPATCHER_IMPL_H__
