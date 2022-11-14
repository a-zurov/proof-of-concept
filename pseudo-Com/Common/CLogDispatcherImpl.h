//////////////////////////////////////////////////////////////////////////
// Module: CLogDispatcherImpl - pseudo COM's logs support system
//////////////////////////////////////////////////////////////////////////

#ifndef __GCN_CLOG_DISPATCHER_IMPL_H__
#define __GCN_CLOG_DISPATCHER_IMPL_H__

//////////////////////////////////////////////////////////////////////////

#include <set>

#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "AtomicSync.h"
#include "RefCounter.h"
#include "CSmartPtr.h"
#include "Singleton.h"

#include "ILog.h"
#include "ILogHandler.h"
#include "IConnectionPoint.h"

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_BEGIN

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
    virtual ResultCode QueryInterface(const GCN_UUID& uuid, void** ppInterface);

    virtual ResultCode Bind(const GCN_UUID& uuid, void* pInterface);

    virtual ResultCode Unbind(const GCN_UUID& uuid, void* pInterface);

    virtual void Close();

    void AddLogHandler(ILogHandler* pEvents);

    void RemoveLogHandler(ILogHandler* pEvents);

private:
    volatile UINT32_t m_nLogLevel = LL_PREBIND;
    LogHandlersSet_t m_setLogHandlers;
    boost::shared_mutex m_RWEventsLock;
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

    virtual ResultCode QueryInterface(const GCN_UUID& uuid, void** ppInterface);

private:
    LogDispatcherPtr_t m_spLogDispatcher;
};

//////////////////////////////////////////////////////////////////////////

#include "DumpFunction.hpp"

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif //__GCN_CLOG_DISPATCHER_IMPL_H__
