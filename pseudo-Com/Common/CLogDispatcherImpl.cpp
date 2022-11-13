//////////////////////////////////////////////////////////////////////////
// Module: LogHandlerImpl.cpp - The log support system.
//////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "framework.h"

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_BEGIN

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

    sync::upgrade_guard_t upLock(m_RWEventsLock);

    if (m_setLogHandlers.empty()) {

        sync::upto_write_guard_t wLock(upLock);

        std::cout << szMessage << std::endl;

        return;
    }

    for (LogHandlersSet_t::const_iterator it = m_setLogHandlers.begin(), end = m_setLogHandlers.end(); it != end; ++it)
    {
        ILogHandler* pEvent = *it;

        sync::upto_write_guard_t wLock(upLock);

        if (pEvent != NULL) pEvent->OnLogMessage(nLogLevel, szMessage.c_str());
    }
}

ResultCode CLogDispatcherImpl::QueryInterface(const GCN_UUID& uuid, void** ppInterface)
{
    if (NULL == ppInterface)
    {
        return CC_INVALID_PARAMETER;
    }

    if (EqualsUUID(uuid, IConnectionPointContainer_UUID))
    {
        IConnectionPointContainer* pCPC = static_cast<IConnectionPointContainer*>(this);

        (*ppInterface) = reinterpret_cast<void*>(pCPC);

        return CC_OK;
    }

    return CC_UNKNOWN_INTERFACE;
}

ResultCode CLogDispatcherImpl::Bind(const GCN_UUID& uuid, void* pInterface)
{
    if (NULL != pInterface)
    {
        if (EqualsUUID(uuid, ILogHandler_UUID))
        {
            AddLogHandler(reinterpret_cast<ILogHandler*>(pInterface));

            return CC_OK;
        }
    }

    return CC_UNKNOWN_INTERFACE;
}

ResultCode CLogDispatcherImpl::Unbind(const GCN_UUID& uuid, void* pInterface)
{
    if (NULL != pInterface)
    {
        if (EqualsUUID(uuid, ILogHandler_UUID))
        {
            RemoveLogHandler(reinterpret_cast<ILogHandler*>(pInterface));

            return CC_OK;
        }
    }

    return CC_UNKNOWN_INTERFACE;
}

void CLogDispatcherImpl::AddLogHandler(ILogHandler* pEvents)
{
    sync::write_guard_t wLock(m_RWEventsLock);

    m_setLogHandlers.insert(pEvents);
}

void CLogDispatcherImpl::RemoveLogHandler(ILogHandler* pEvents)
{
    sync::write_guard_t wLock(m_RWEventsLock);

    m_setLogHandlers.erase(pEvents);
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

ResultCode CLogImpl::QueryInterface(const GCN_UUID& uuid, void** ppInterface)
{
    if (nullptr == ppInterface)
    {
        return CC_INVALID_PARAMETER;
    }

    if (EqualsUUID(uuid, ILogDispatcher_UUID))
    {
        if (!m_spLogDispatcher) m_spLogDispatcher = new CLogDispatcherImpl();

        ILogDispatcher* pInterface = static_cast<ILogDispatcher*>(static_cast<CLogDispatcherImpl*>(m_spLogDispatcher));

        pInterface->AddRef();

        (*ppInterface) = reinterpret_cast<void*>(pInterface);

        return CC_OK;
    }

    if (EqualsUUID(uuid, ILogDispatcherSingltone_UUID))
    {
        ILogDispatcher* pInterface = static_cast<ILogDispatcher*>(&LogDispatcherSingltone::instance());

        (*ppInterface) = reinterpret_cast<void*>(pInterface);

        return CC_OK;
    }

    return CC_UNKNOWN_INTERFACE;
}

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
