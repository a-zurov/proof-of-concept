//////////////////////////////////////////////////////////////////////////
// Module: LogHandlerImpl.cpp - The log support system.
//////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "framework.h"

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////

CLogHandlerImpl::~CLogHandlerImpl()
{

}

CLogHandlerImpl::CLogHandlerImpl()
{

}

void CLogHandlerImpl::SetLogLevel(const LogLevel nLogLevel)
{
    sync::AtomicWrite32(m_nLogLevel, (UINT32_t)nLogLevel);
}

long CLogHandlerImpl::AddRef()
{
    return CRefCounter::AddRef();
}

long CLogHandlerImpl::Release()
{
    return CRefCounter::Release();
}

void CLogHandlerImpl::Close()
{

}

LogLevel CLogHandlerImpl::GetLogLevel()
{
    return (LogLevel)sync::AtomicRead32(m_nLogLevel);
}

void CLogHandlerImpl::FireLogMessage(const LogLevel nLogLevel, const std::string& sMessage)
{

    auto nLevel = GetLogLevel();

    if (nLevel < nLogLevel)
    {
        return;
    }

    //sync::read_guard_t rGuard( m_RWEventsLock );

    sync::upgrade_guard_t upGuard(m_RWEventsLock);

    if (m_setEvents.empty()) {

        sync::upto_write_guard_t lock(upGuard);
        std::cout << sMessage << std::endl;
        return;
    }

    for (EventsSet_t::iterator it = m_setEvents.begin(), end = m_setEvents.end(); it != end; ++it)
    {
        ILogHandlerEvents* pEvents = *it;

        if (pEvents != NULL) pEvents->OnLogMessage(nLogLevel, sMessage.c_str());
    }
}

ResultCode CLogHandlerImpl::QueryInterface(const GCN_UUID& uuid, void** ppInterface)
{
    if (NULL == ppInterface)
    {
        return CC_INVALID_PARAMETER;
    }

    if (EqualsUUID(uuid, GCN_ICP_UUID))
    {
        IConnectionPoint* pCPC = static_cast<IConnectionPoint*>(this);

        (*ppInterface) = reinterpret_cast<void*>(pCPC);

        return CC_OK;
    }

    return CC_UNKNOWN_INTERFACE;
}

ResultCode CLogHandlerImpl::Bind(const GCN_UUID& uuid, void* pInterface)
{
    if (NULL != pInterface)
    {
        if (EqualsUUID(uuid, ILogHandlerEvents_UUID))
        {
            AddEvents(reinterpret_cast<ILogHandlerEvents*>(pInterface));

            return CC_OK;
        }
    }

    return CC_UNKNOWN_INTERFACE;
}

ResultCode CLogHandlerImpl::Unbind(const GCN_UUID& uuid, void* pInterface)
{
    if (NULL != pInterface)
    {
        if (EqualsUUID(uuid, ILogHandlerEvents_UUID))
        {
            RemoveEvents(reinterpret_cast<ILogHandlerEvents*>(pInterface));

            return CC_OK;
        }
    }

    return CC_UNKNOWN_INTERFACE;
}

void CLogHandlerImpl::AddEvents(ILogHandlerEvents* pEvents)
{
    sync::write_guard_t wLock(m_RWEventsLock);

    m_setEvents.insert(pEvents);
}

void CLogHandlerImpl::RemoveEvents(ILogHandlerEvents* pEvents)
{
    sync::write_guard_t wLock(m_RWEventsLock);

    m_setEvents.erase(pEvents);
}

CLoggerImpl::~CLoggerImpl()
{

}

CLoggerImpl::CLoggerImpl()
{

}

long CLoggerImpl::AddRef()
{
    return CRefCounter::AddRef();
}

long CLoggerImpl::Release()
{
    return CRefCounter::Release();
}

void CLoggerImpl::Close()
{

}

ResultCode CLoggerImpl::QueryInterface(const GCN_UUID& uuid, void** ppInterface)
{
    if (nullptr == ppInterface)
    {
        return CC_INVALID_PARAMETER;
    }

    if (EqualsUUID(uuid, ILogHandler_UUID))
    {
        if (!m_spLogHandler) m_spLogHandler = new CLogHandlerImpl();

        ILogHandler* pInterface = static_cast<ILogHandler*>(static_cast<CLogHandlerImpl*>(m_spLogHandler));

        pInterface->AddRef();

        (*ppInterface) = reinterpret_cast<void*>(pInterface);

        return CC_OK;
    }

    if (EqualsUUID(uuid, ILogHandlerSingltone_UUID))
    {
        ILogHandler* pInterface = static_cast<ILogHandler*>(&LogHandlerSingltone::instance());

        (*ppInterface) = reinterpret_cast<void*>(pInterface);

        return CC_OK;
    }

    return CC_UNKNOWN_INTERFACE;
}

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
