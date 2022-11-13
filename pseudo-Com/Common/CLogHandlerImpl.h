//////////////////////////////////////////////////////////////////////////
// Module: LogHandlerImpl.h - The log's support system.
//////////////////////////////////////////////////////////////////////////

#ifndef __TEST_LIB_LOG_HANDLER_IMPL_H__
#define __TEST_LIB_LOG_HANDLER_IMPL_H__

//////////////////////////////////////////////////////////////////////////

#include <string>
#include <set>

#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "AtomicSync.h"
#include "RefCounter.h"
#include "CSmartPtr.h"
#include "IConnectionPoint.h"
#include "Singleton.h"

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////

class CLogHandlerImpl
    : public ILogHandler
    , protected IConnectionPoint
    , protected CRefCounter
{
    typedef std::set< ILogHandlerEvents* > EventsSet_t;

public:
    CLogHandlerImpl();

    virtual ~CLogHandlerImpl();

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

    void AddEvents(ILogHandlerEvents* pEvents);

    void RemoveEvents(ILogHandlerEvents* pEvents);

private:
    volatile UINT32_t m_nLogLevel = GCN_LL_FUNC;
    EventsSet_t m_setEvents;
    boost::shared_mutex m_RWEventsLock;
};

typedef singleton< CLogHandlerImpl > LogHandlerSingltone;

typedef CSmartPtr< CLogHandlerImpl > LogHandlerPtr_t;

class CLoggerImpl
    : public ILogger
    , protected CRefCounter
{
public:
    CLoggerImpl();

    virtual ~CLoggerImpl();

    virtual long AddRef();

    virtual long Release();

    virtual void Close();

    virtual ResultCode QueryInterface(const GCN_UUID& uuid, void** ppInterface);

private:
    LogHandlerPtr_t m_spLogHandler;
};

//////////////////////////////////////////////////////////////////////////

class DumpFunction
{
public:
    DumpFunction(const char* pchSourceFile, const char* pchFunctionName, int nSourceLine)
    {
        m_szSourceFile.assign(pchSourceFile);

        std::size_t nPos = m_szSourceFile.rfind(__DELIM__);

        if (std::string::npos != nPos)
        {
            m_szSourceFile.assign(m_szSourceFile.substr(nPos + 1));
        }

        m_nSourceLine = nSourceLine;
        m_szFunctionName = pchFunctionName;

        std::string szMessage("ThreadID: ");
        szMessage.append(boost::lexical_cast<std::string>(boost::this_thread::get_id()));
        szMessage.append(": Enter >> Source: ");
        szMessage.append(m_szSourceFile);
        szMessage.append("( ");
        szMessage.append(boost::lexical_cast<std::string>(m_nSourceLine));
        szMessage.append(" ) | ");
        szMessage.append(m_szFunctionName);
        LogHandlerSingltone::instance().FireLogMessage(GCN_LL_FUNC, szMessage);
    }

    ~DumpFunction()
    {
        std::string szMessage("ThreadID: ");
        szMessage.append(boost::lexical_cast<std::string>(boost::this_thread::get_id()));
        szMessage.append(": Exit <<< Source: ");
        szMessage.append(m_szSourceFile);
        szMessage.append("( ");
        szMessage.append(boost::lexical_cast<std::string>(m_nSourceLine));
        szMessage.append(" ) | ");
        szMessage.append(m_szFunctionName);
        LogHandlerSingltone::instance().FireLogMessage(GCN_LL_FUNC, szMessage);
    }

private:
    std::string m_szSourceFile;
    std::string m_szFunctionName;

    int m_nSourceLine;
};

#define DUMP_FUNCTION()                                         \
    DumpFunction DumpFunc( __FILE__, __FUNCTION__, __LINE__ )

#define DUMP_EXCEPTION( ex ) {                                                              \
    std::string sMessage( "ThreadID: " );                                                   \
    sMessage.append( boost::lexical_cast< std::string >( boost::this_thread::get_id() ) );  \
    sMessage.append( ": " );                                                                \
    sMessage.append( ex.what() );                                                           \
    LogHandlerSingltone::instance().FireLogMessage( CC_LL_ERROR, sMessage );                \
}

#define DUMP_BOOST_EXCEPTION( ex ) {                                                        \
    std::string sMessage( "ThreadID: " );                                                   \
    sMessage.append( boost::lexical_cast< std::string >( boost::this_thread::get_id() ) );  \
    sMessage.append( ": " );                                                                \
    sMessage.append( boost::diagnostic_information_what( ex ) );                            \
    LogHandlerSingltone::instance().FireLogMessage( CC_LL_ERROR, sMessage );                \
}

#define DUMP_MESSAGE( ll, message ) {                                                           \
    if( ll <= LogHandlerSingltone::instance().GetLogLevel() )                                   \
    {                                                                                           \
        std::string sSourceFile( __FILE__ );                                                    \
                                                                                                \
        std::size_t nPos = sSourceFile.rfind( PATH_SLASH );                                     \
                                                                                                \
        if( std::string::npos != nPos )                                                         \
        {                                                                                       \
            sSourceFile.assign( sSourceFile.substr( nPos + 1 ) );                               \
        }                                                                                       \
                                                                                                \
        std::string sMessage( "ThreadID: " );                                                   \
        sMessage.append( boost::lexical_cast< std::string >( boost::this_thread::get_id() ) );  \
        sMessage.append( ": " );                                                                \
        sMessage.append( "Message: Source" );                                                   \
        sMessage.append( ": " );                                                                \
        sMessage.append( sSourceFile );                                                         \
        sMessage.append( "( " );                                                                \
        sMessage.append( boost::lexical_cast< std::string >( __LINE__ ) );                      \
        sMessage.append( " )" );                                                                \
        sMessage.append( " | " );                                                               \
        sMessage.append( __FUNCTION__ );                                                        \
        sMessage.append( " " );                                                                 \
        std::ostringstream stream;                                                              \
        stream << sMessage << message;                                                          \
        sMessage.assign( stream.str() );                                                        \
        m_spLogHandler->FireLogMessage( ll, sMessage );                                         \
    }                                                                                           \
}

#define DUMP_INFO( message ) \
    DUMP_MESSAGE( CC_LL_INFO, message )

#define DUMP_WARNING( message ) \
    DUMP_MESSAGE( CC_LL_WARNING, message )

#define DUMP_ERROR( message ) \
    DUMP_MESSAGE( CC_LL_ERROR, message )

#define DUMP_DEBUG( message ) \
    DUMP_MESSAGE( CC_LL_DEBUG, message )


//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __TEST_LIB_LOG_HANDLER_IMPL_H__
