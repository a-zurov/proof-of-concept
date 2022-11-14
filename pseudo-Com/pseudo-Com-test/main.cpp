// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>

#include <thread>
#include <chrono>
#include <mutex>

#include "../Common/IBase.h"
#include "../Common/RefCounter.h"
#include "../Common/CSmartPtr.h"
#include "../Common/ILogHandler.h"
#include "../Common/IConnectionPoint.h"
#include "../Common/DumpFunction.hpp"

//////////////////////////////////////////////////////////////////////////

#define THROW_EXCEPTION( message ) {                                                \
    std::ostringstream oss;                                                         \
    oss << __LINE__ << ":" __FILE__ << "::" << __FUNCTION__ << "(): " << message;   \
    std::string szMessage( oss.str() );                                             \
    throw std::runtime_error( szMessage ); }

//////////////////////////////////////////////////////////////////////////

class CLogHandlerImpl
    : public xcom::ILogHandler
{
public:
    void FireLogMessage(const char* pMessage)
    {
        std::cout << pMessage << std::endl;
    }

protected:
    virtual void OnLogMessage(const xcom::LogLevel nLogLevel, const char* pMessage)
    {
        FireLogMessage(pMessage);
    }
};

static CLogHandlerImpl g_LogHandler;

//////////////////////////////////////////////////////////////////////////

class IComObject
    : public xcom::IBase
{
public:
    virtual void Foo() const = 0;
};

class CComObjectImpl
    : public IComObject
    , protected xcom::CRefCounter
{
    using ThisRefCounter = xcom::CRefCounter;

    mutable std::mutex m_mtxFooCounter;

    mutable int m_nFooCounter = 0;

public:
    virtual ~CComObjectImpl() {}

    CComObjectImpl() {}

    long AddRef() override;

    long Release() override;

    void Close() override
    {
        DUMP_MESSAGE(xcom::LL_WARNING , "ThisRefCounter is destroying CComObjectImpl");
    };

    void Foo() const override
    {
        DUMP_FUNCTION();

        std::lock_guard<std::mutex> lock(m_mtxFooCounter);

        DUMP_INFO(" CComObjectImpl::m_nFooCounter = " << ++m_nFooCounter);
    };
};

long CComObjectImpl::AddRef()
{
    auto nRef = ThisRefCounter::AddRef();

    DUMP_MESSAGE(xcom::LL_INFO, " ThisRefCounter = " << nRef);

    return nRef;
}

long CComObjectImpl::Release()
{
    auto nRef = ThisRefCounter::Release();

    DUMP_MESSAGE(xcom::LL_INFO, " ThisRefCounter = " << nRef);

    return nRef;
}

using ComObjectPtr_t = xcom::CSmartPtr< IComObject >;

typedef xcom::CSmartPtr< xcom::ILog > LogPtr_t;
typedef xcom::CSmartPtr< xcom::ILogDispatcher > LogDispatcherPtr_t;


void thread_func(ComObjectPtr_t spObject, LogDispatcherPtr_t spLogDispatcher)
{
    DUMP_FUNCTION_TO(spLogDispatcher);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    ComObjectPtr_t spComObject;
    spComObject.Attach(spObject.Detach()); // thread-safe due to atomic, even though the ref-counter is incremented

    {
        spComObject->Foo();
    }
}


int main()
{
    LogPtr_t spLog;

    LogDispatcherPtr_t spLogDispatcher;

    spLog.Attach( xcom::CreateLog() );

#ifdef ALLOW_SINGLETON_DISPATCH_LOG

    xcom::ILogDispatcher* pLogDispatcher = nullptr;

    xcom::ResultCode nResult = spLog->QueryInterface(xcom::ILogDispatcherSingleton_UUID,
                                        reinterpret_cast<void**>(&pLogDispatcher));

    if (xcom::OK != nResult)
    {
        THROW_EXCEPTION("Cannot query ILogDispatcher Singleton interface!");
    }

    pLogDispatcher->SetLogLevel(xcom::LL_PREBIND);

    xcom::IConnectionPointContainer* pConnectionPointContainer = nullptr;

    if (xcom::OK != (nResult = pLogDispatcher->QueryInterface(xcom::IConnectionPointContainer_UUID,
                                        reinterpret_cast<void**>(&pConnectionPointContainer))))
    {
        THROW_EXCEPTION("Cannot query ConnectionPointContainer interface!");
    }

#else //ALLOW_SINGLETON_DISPATCH_LOG

    xcom::ResultCode nResult = spLog->QueryInterface(xcom::ILogDispatcher_UUID,
                                        reinterpret_cast<void**>(&spLogDispatcher));

    if (xcom::OK != nResult)
    {
        THROW_EXCEPTION("Cannot query ILogDispatcher interface!");
    }


    xcom::IConnectionPointContainer* pConnectionPointContainer = nullptr;

    if (xcom::OK != (nResult = spLogDispatcher->QueryInterface(xcom::IConnectionPointContainer_UUID,
                                                        reinterpret_cast<void**>(&pConnectionPointContainer))))
    {
        THROW_EXCEPTION("Cannot query ConnectionPointContainer interface!");
    }

#endif //ALLOW_SINGLETON_DISPATCH_LOG

    if (xcom::OK != (nResult = pConnectionPointContainer->Bind(xcom::ILogHandler_UUID,
                                                        reinterpret_cast<void*>(static_cast<xcom::ILogHandler*>(&g_LogHandler)))))
    {
        THROW_EXCEPTION("Cannot bind LogHandler interface!");
    }


    checkpoint("Start");

    ComObjectPtr_t spComObject( static_cast<IComObject*>(new CComObjectImpl()) );

    {
        std::thread t1(thread_func, spComObject, spLogDispatcher),
                    t2(thread_func, spComObject, spLogDispatcher),
                    t3(thread_func, spComObject, spLogDispatcher);

        spComObject.Release();

        t1.join(); t2.join(); t3.join();
    }

    checkpoint("Stop");

    spLogDispatcher.Release();

    spLog.Release();
}

//////////////////////////////////////////////////////////////////////////
