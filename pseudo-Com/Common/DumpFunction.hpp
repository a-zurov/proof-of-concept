//////////////////////////////////////////////////////////////////////////
// Module: DumpFunction.hpp
//////////////////////////////////////////////////////////////////////////

#ifndef __GCN_DUMP_FUNCTION_HPP__
#define __GCN_DUMP_FUNCTION_HPP__

#include "CLogDispatcherImpl.h"

#include <string>
#include <sstream>

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////

class DumpFunction final
{
public:
    DumpFunction(const char* pSource, const char* pFunction, int nLine, CLogDispatcherImpl* p = nullptr)
    {
        m_szSourceFile.assign(pSource);

        std::size_t nPos = m_szSourceFile.rfind(__DELIM__);

        if (std::string::npos != nPos)
        {
            m_szSourceFile.assign(m_szSourceFile.substr(nPos + 1));
        }

        m_nSourceLine = nLine;
        m_szFunctionName = pFunction;

        std::string szMessage("ThrID: ");
        szMessage.append(boost::lexical_cast<std::string>(boost::this_thread::get_id()));
        szMessage.append(": Enter >> Source: ");
        szMessage.append(m_szSourceFile);
        szMessage.append("( ");
        szMessage.append(boost::lexical_cast<std::string>(m_nSourceLine));
        szMessage.append(" ) | ");
        szMessage.append(m_szFunctionName);

#ifdef ALLOW_SINGLETON_DISPATCH_LOG
        LogDispatcherSingleton::instance().FireLogMessage(LL_PREBIND, szMessage);
#else
        m_spLogDispatcher = p;
        if (m_spLogDispatcher) static_cast<CLogDispatcherImpl*>(m_spLogDispatcher)->FireLogMessage(LL_PREBIND, szMessage);
#endif
    }

    ~DumpFunction()
    {
        std::string szMessage("ThrID: ");
        szMessage.append(boost::lexical_cast<std::string>(boost::this_thread::get_id()));
        szMessage.append(": Exit <<< Source: ");
        szMessage.append(m_szSourceFile);
        szMessage.append("( ");
        szMessage.append(boost::lexical_cast<std::string>(m_nSourceLine));
        szMessage.append(" ) | ");
        szMessage.append(m_szFunctionName);

#ifdef ALLOW_SINGLETON_DISPATCH_LOG
        LogDispatcherSingleton::instance().FireLogMessage(LL_PREBIND, szMessage);
#else
        if (m_spLogDispatcher) static_cast<CLogDispatcherImpl*>(m_spLogDispatcher)->FireLogMessage(LL_PREBIND, szMessage);
#endif

    }

private:
    std::string m_szSourceFile;
    std::string m_szFunctionName;

    int m_nSourceLine;

#ifndef ALLOW_SINGLETON_DISPATCH_LOG
    LogDispatcherPtr_t m_spLogDispatcher;
#endif
};

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#ifdef ALLOW_PREBIND_DUMP
    #define PREBIND_DUMP_FUNCTION() gcn::DumpFunction DumpFunc( __FILE__, __FUNCTION__, __LINE__ )
#else
    #define PREBIND_DUMP_FUNCTION()
#endif

#ifdef ALLOW_SINGLETON_DISPATCH_LOG

    #define DUMP_FUNCTION() gcn::DumpFunction DumpFunc( __FILE__, __FUNCTION__, __LINE__ )
    #define DUMP_FUNCTION_TO(x) gcn::DumpFunction DumpFunc(__FILE__, __FUNCTION__, __LINE__)

    #define DUMP_MESSAGE( ll, message ) {                                                       \
    if( ll <= gcn::LogDispatcherSingleton::instance().GetLogLevel() )                           \
    {                                                                                           \
        std::string szSourceFile( __FILE__ );                                                   \
                                                                                                \
        std::size_t nPos = szSourceFile.rfind( __DELIM__ );                                     \
                                                                                                \
        if( std::string::npos != nPos )                                                         \
        {                                                                                       \
            szSourceFile.assign( szSourceFile.substr( nPos + 1 ) );                             \
        }                                                                                       \
                                                                                                \
        std::string szMessage( "ThrID: " );                                                     \
        szMessage.append( boost::lexical_cast< std::string >( boost::this_thread::get_id() ) ); \
        szMessage.append( ": " );                                                               \
        szMessage.append( "Message: Source" );                                                  \
        szMessage.append( ": " );                                                               \
        szMessage.append( szSourceFile );                                                       \
        szMessage.append( "( " );                                                               \
        szMessage.append( boost::lexical_cast< std::string >( __LINE__ ) );                     \
        szMessage.append( " )" );                                                               \
        szMessage.append( " | " );                                                              \
        szMessage.append( __FUNCTION__ );                                                       \
        szMessage.append( " " );                                                                \
        std::ostringstream oss;                                                                 \
        oss << szMessage << message;                                                            \
        szMessage.assign( oss.str() );                                                          \
        gcn::LogDispatcherSingleton::instance().FireLogMessage( ll, szMessage );                \
    }                                                                                           \
}

#else
    #define DUMP_FUNCTION()
    #define DUMP_FUNCTION_TO(x) gcn::DumpFunction DumpFunc(__FILE__, __FUNCTION__, __LINE__, static_cast<gcn::CLogDispatcherImpl*>((gcn::ILogDispatcher*)x))
    #define DUMP_MESSAGE( ll, message )
#endif


#define DUMP_INFO( message ) \
    DUMP_MESSAGE( gcn::LL_INFO, message )

#define DUMP_WARNING( message ) \
    DUMP_MESSAGE( gcn::LL_WARNING, message )

#define DUMP_ERROR( message ) \
    DUMP_MESSAGE( gcn::LL_ERROR, message )

#define DUMP_DEBUG( message ) \
    DUMP_MESSAGE( gcn::LL_DEBUG, message )

/*
#define DUMP_EXCEPTION( ex ) {                                                              \
    std::string sMessage( "ThreadID: " );                                                   \
    sMessage.append( boost::lexical_cast< std::string >( boost::this_thread::get_id() ) );  \
    sMessage.append( ": " );                                                                \
    sMessage.append( ex.what() );                                                           \
    gcn::LogDispatcherSingleton::instance().FireLogMessage( LL_ERROR, sMessage );       \
}

#define DUMP_BOOST_EXCEPTION( ex ) {                                                        \
    std::string sMessage( "ThreadID: " );                                                   \
    sMessage.append( boost::lexical_cast< std::string >( boost::this_thread::get_id() ) );  \
    sMessage.append( ": " );                                                                \
    sMessage.append( boost::diagnostic_information_what( ex ) );                            \
    gcn::LogDispatcherSingleton::instance().FireLogMessage( LL_ERROR, sMessage );       \
}
*/



#endif //__GCN_DUMP_FUNCTION_HPP__
