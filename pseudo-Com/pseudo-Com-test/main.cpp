// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <sstream>

#define MAKE_COUT_DUMP  // NOT thread-safe

#include "../Common/IBase.h"
#include "../Common/RefCounter.h"
#include "../Common/CSmartPtr.h"
#include "../Common/ILogHandler.h"
#include "../Common/IConnectionPoint.h"
#include "../Common/DumpFunction.hpp"

#define THROW_EXCEPTION( sMessage ) {                                                           \
    std::ostringstream sMessageStream;                                                          \
    sMessageStream << __LINE__ << ":" __FILE__ << "::" << __FUNCTION__ << "(): " << sMessage;   \
    std::string sMsg( sMessageStream.str() );                                                   \
    throw std::runtime_error( sMsg ); }

class CLogHandlerImpl
    : public gcn::ILogHandler
{
public:
    void FireLogMessage(const char* pchMessage)
    {
        std::cout << pchMessage << std::endl;
    }

protected:
    virtual void OnLogMessage(const gcn::LogLevel nLogLevel, const char* pchMessage)
    {
        FireLogMessage(pchMessage);
    }
};

static CLogHandlerImpl g_LogHandler;

class IComObject
    : public gcn::IBase
{
public:
    virtual void Foo() = 0;
};

class CComObjectImpl
    : public IComObject
    , protected gcn::CRefCounter
{
    using ThisRefCounter = gcn::CRefCounter;

public:
    virtual ~CComObjectImpl()
    {
        cout_dump_this();
    }

    CComObjectImpl()
    {
        cout_dump_this();
    }

    long AddRef() override;

    long Release() override;

    void Close() override
    {
#ifdef MAKE_COUT_DUMP
        cout_dump_msg("the container is decaying right now");
#else
        std::cout << "the container is decaying right now" << std::endl;
#endif
    };

    void Foo() override
    {
#ifdef MAKE_COUT_DUMP
        cout_dump_msg( ThisRefCounter::get() );
#else
        std::cout << "Foo() call RefCounter = " << ThisRefCounter::get() << std::endl;
#endif
    };
};

long CComObjectImpl::AddRef()
{
    auto nRef = ThisRefCounter::AddRef();

    cout_dump_msg(nRef);

    return nRef;
}

long CComObjectImpl::Release()
{
    auto nRef = ThisRefCounter::Release();

    cout_dump_msg(nRef);

    return nRef;
}

using ComObjectPtr_t = gcn::CSmartPtr< IComObject >;
typedef gcn::CSmartPtr< gcn::ILog > LogPtr_t;
typedef gcn::CSmartPtr< gcn::ILogDispatcher > LogDispatcherPtr_t;

void thread_func(ComObjectPtr_t spObject, LogDispatcherPtr_t spLogDispatcher)
{
    DUMP_FUNCTION_TO(spLogDispatcher);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    ComObjectPtr_t spComObject;
    spComObject.Attach(spObject.Detach()); // thread-safe, even though the counter is incremented

    {
        static std::mutex io_mutex;
        std::lock_guard<std::mutex> lock(io_mutex);

        spComObject->Foo();
    }
}

int main()
{
    LogPtr_t spLog;
    LogDispatcherPtr_t spLogDispatcher;

    spLog.Attach(gcn::CreateLog());

#ifdef ALLOW_SINGLTONE_LOG_DISPATCH

    gcn::ILogDispatcher* pLogDispatcherSingltone = nullptr;

    gcn::ResultCode nResult = spLog->QueryInterface(gcn::ILogDispatcherSingltone_UUID,
                                        reinterpret_cast<void**>(&pLogDispatcherSingltone));

    if (gcn::GCN_OK != nResult)
    {
        THROW_EXCEPTION("Cannot query ILogDispatcherSingltone interface!");
    }

    pLogDispatcherSingltone->SetLogLevel(gcn::GCN_LL_FUNC);

    gcn::IConnectionPointContainer* pConnectionPointContainer = nullptr;

    if (gcn::GCN_OK != (nResult = pLogDispatcherSingltone->QueryInterface(gcn::IConnectionPointContainer_UUID,
        reinterpret_cast<void**>(&pConnectionPointContainer))))
    {
        THROW_EXCEPTION("Cannot query ConnectionPointContainer interface!");
    }

#else //ALLOW_SINGLTONE_LOG_DISPATCH

    gcn::ResultCode nResult = spLog->QueryInterface(gcn::ILogDispatcher_UUID,
                                        reinterpret_cast<void**>(&spLogDispatcher));

    if (gcn::GCN_OK != nResult)
    {
        THROW_EXCEPTION("Cannot query ILogDispatcherSingltone interface!");
    }

    spLogDispatcher->SetLogLevel(gcn::GCN_LL_FUNC);

    gcn::IConnectionPointContainer* pConnectionPointContainer = nullptr;

    if (gcn::GCN_OK != (nResult = spLogDispatcher->QueryInterface(gcn::IConnectionPointContainer_UUID,
                                                        reinterpret_cast<void**>(&pConnectionPointContainer))))
    {
        THROW_EXCEPTION("Cannot query ConnectionPointContainer interface!");
    }

#endif //ALLOW_SINGLTONE_LOG_DISPATCH

    if (gcn::GCN_OK != (nResult = pConnectionPointContainer->Bind(gcn::ILogHandler_UUID,
                                                        reinterpret_cast<void*>(static_cast<gcn::ILogHandler*>(&g_LogHandler)))))
    {
        THROW_EXCEPTION("Cannot bind LogHandler interface!");
    }

    checkpoint("Start");

    ComObjectPtr_t spComObject(static_cast<IComObject*>(new CComObjectImpl()));

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
