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

/*
#define DUMP_MESSAGE( __ex )    g_LogHandler.FireLogMessage( __ex ) //??????????????
#define DUMP_EXCEPTION( __ex )  DUMP_MESSAGE( __ex.what() )
*/

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

void thread_func(ComObjectPtr_t sptr)
{

#ifndef MAKE_COUT_DUMP
    std::this_thread::sleep_for(std::chrono::seconds(1));
#endif

    ComObjectPtr_t spComObject;
    spComObject.Attach(sptr.Detach()); // thread-safe, even though the counter is incremented

    {
        static std::mutex io_mutex;
        std::lock_guard<std::mutex> lock(io_mutex);

        spComObject->Foo();
    }
}

typedef gcn::CSmartPtr< gcn::ILog > LogPtr_t;
typedef gcn::CSmartPtr< gcn::ILogDispatcher > LogDispatcherPtr_t;
typedef gcn::CSmartPtr< gcn::IConnectionPointContainer > ConnectionPointPtr_t;

int main()
{
    LogPtr_t spLogger;

    spLogger.Attach(gcn::CreateLog());

    LogDispatcherPtr_t spLogHandler;

    gcn::ILogDispatcher* pLogDispatcherSingltone = nullptr;

    gcn::ResultCode nResult = spLogger->QueryInterface(gcn::ILogDispatcherSingltone_UUID, reinterpret_cast<void**>(&pLogDispatcherSingltone));

    if (gcn::CC_OK != nResult)
    {
        THROW_EXCEPTION("Cannot query LogHandler interface!");
    }

    ConnectionPointPtr_t spCPC;

    if (gcn::CC_OK != (nResult = pLogDispatcherSingltone->QueryInterface(gcn::IConnectionPointContainer_UUID, reinterpret_cast<void**>(&spCPC))))
    {
        THROW_EXCEPTION("Cannot query ConnectionPointContainer interface!");
    }

    /*
    if (gcn::CC_OK != (nResult = spLogHandler->QueryInterface(gcn::GCN_ICP_UUID, reinterpret_cast<void**>(&spCPC))))
    {
        THROW_EXCEPTION("Cannot query ConnectionPointContainer interface!");
    }
    */
    if (gcn::CC_OK != (nResult = spCPC->Bind(gcn::ILogHandler_UUID,
                        reinterpret_cast<void*>(static_cast<gcn::ILogHandler*>(&g_LogHandler)))))
    {
        THROW_EXCEPTION("Cannot bind LogHandlerEvents interface!");
    }

    //spLogHandler->SetLogLevel(gcn::GCN_LL_FUNC);

    pLogDispatcherSingltone->SetLogLevel(gcn::GCN_LL_FUNC);

    checkpoint("Start");

    ComObjectPtr_t spComObject(static_cast<IComObject*>(new CComObjectImpl()));

    {

#ifdef MAKE_COUT_DUMP

        thread_func(spComObject);

        spComObject.Release();

#else

        std::thread t1(thread_func, spComObject),
                    t2(thread_func, spComObject),
                    t3(thread_func, spComObject);

        spComObject.Release();

        t1.join(); t2.join(); t3.join();

#endif // MAKE_COUT_DUMP
    }

    checkpoint("Stop");

    spCPC.Release();

    spLogHandler.Release();

    spLogger.Release();
}
