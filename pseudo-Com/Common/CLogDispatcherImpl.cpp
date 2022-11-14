//////////////////////////////////////////////////////////////////////////
// Module: LogHandlerImpl.cpp - The log support system.
//////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "framework.h"

//////////////////////////////////////////////////////////////////////////

XCOM_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////

CLogDispatcherImpl::~CLogDispatcherImpl()
{

}

CLogDispatcherImpl::CLogDispatcherImpl()
{

}

void CLogDispatcherImpl::SetLogLevel(const LogLevel nLogLevel)
{
    sync::AtomicWrite32(m_nLogLevel, (UINT32_t)nLogLevel);
}

long CLogDispatcherImpl::AddRef()
{
    return CRefCounter::AddRef();
}

long CLogDispatcherImpl::Release()
{
    return CRefCounter::Release();
}

void CLogDispatcherImpl::Close()
{

}

LogLevel CLogDispatcherImpl::GetLogLevel()
{
    return (LogLevel)sync::AtomicRead32(m_nLogLevel);
}

void CLogDispatcherImpl::FireLogMessage(const LogLevel nLogLevel, const std::string& szMessage)
{

    auto nLevel = GetLogLevel();

    if (nLevel < nLogLevel)
    {
        return;
    }

    sync::upgrade_guard_t upLock(m_mtxLogHanlersGuard);

#ifdef ALLOW_PREBIND_DUMP

    if (m_setLogHandlers.empty()) {

        sync::upto_write_guard_t wLock(upLock);

        std::cout << szMessage << std::endl;

        return;
    }
#endif //ALLOW_PREBIND_DUMP

    for (LogHandlersSet_t::const_iterator it = m_setLogHandlers.begin(), end = m_setLogHandlers.end(); it != end; ++it)
    {
        ILogHandler* pLogHandler = *it;

        sync::upto_write_guard_t wLock(upLock);

        if ( nullptr != pLogHandler) pLogHandler->OnLogMessage(nLogLevel, szMessage.c_str());
    }
}

ResultCode CLogDispatcherImpl::QueryInterface(const XCOM_UUID& uuid, void** ppInterface)
{
    if (nullptr == ppInterface)
    {
        return INVALID_PARAMETER;
    }

    if (EqualsUUID(uuid, IConnectionPointContainer_UUID))
    {
        IConnectionPointContainer* pCPC = static_cast<IConnectionPointContainer*>(this);

        (*ppInterface) = reinterpret_cast<void*>(pCPC);

        return OK;
    }

    return UNKNOWN_INTERFACE;
}

ResultCode CLogDispatcherImpl::Bind(const XCOM_UUID& uuid, void* pInterface)
{
    if (nullptr != pInterface)
    {
        if (EqualsUUID(uuid, ILogHandler_UUID))
        {
            AddLogHandler(reinterpret_cast<ILogHandler*>(pInterface));

            return OK;
        }
    }

    return UNKNOWN_INTERFACE;
}

ResultCode CLogDispatcherImpl::Unbind(const XCOM_UUID& uuid, void* pInterface)
{
    if (nullptr != pInterface)
    {
        if (EqualsUUID(uuid, ILogHandler_UUID))
        {
            RemoveLogHandler(reinterpret_cast<ILogHandler*>(pInterface));

            return OK;
        }
    }

    return UNKNOWN_INTERFACE;
}

void CLogDispatcherImpl::AddLogHandler(ILogHandler* pHandler)
{
    sync::write_guard_t wLock(m_mtxLogHanlersGuard);

    m_setLogHandlers.insert(pHandler);
}

void CLogDispatcherImpl::RemoveLogHandler(ILogHandler* pHandler)
{
    sync::write_guard_t wLock(m_mtxLogHanlersGuard);

    m_setLogHandlers.erase(pHandler);
}

CLogImpl::~CLogImpl()
{

}

CLogImpl::CLogImpl()
{

}

long CLogImpl::AddRef()
{
    return CRefCounter::AddRef();
}

long CLogImpl::Release()
{
    return CRefCounter::Release();
}

void CLogImpl::Close()
{

}

ResultCode CLogImpl::QueryInterface(const XCOM_UUID& uuid, void** ppInterface)
{
    if (nullptr == ppInterface)
    {
        return INVALID_PARAMETER;
    }

    if (EqualsUUID(uuid, ILogDispatcher_UUID))
    {
        if (!m_spLogDispatcher) m_spLogDispatcher = new CLogDispatcherImpl();

        ILogDispatcher* pInterface = static_cast<ILogDispatcher*>(static_cast<CLogDispatcherImpl*>(m_spLogDispatcher));

        pInterface->AddRef();

        (*ppInterface) = reinterpret_cast<void*>(pInterface);

        return OK;
    }

#ifdef ALLOW_SINGLETON_DISPATCH_LOG

    if (EqualsUUID(uuid, ILogDispatcherSingleton_UUID))
    {
        ILogDispatcher* pInterface = static_cast<ILogDispatcher*>(&LogDispatcherSingleton::instance());

        (*ppInterface) = reinterpret_cast<void*>(pInterface);

        return OK;
    }

#endif //ALLOW_SINGLETON_DISPATCH_LOG

    return UNKNOWN_INTERFACE;
}

//////////////////////////////////////////////////////////////////////////

XCOM_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
