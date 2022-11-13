//////////////////////////////////////////////////////////////////////////
// Module: DumpFunction.hpp
//////////////////////////////////////////////////////////////////////////

#ifndef __GCN_DUMP_FUNCTION_HPP__
#define __GCN_DUMP_FUNCTION_HPP__

#include "CLogDispatcherImpl.h"

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////


class DumpFunction
{
public:
    DumpFunction(const char* pchSourceFile, const char* pchFunctionName, int nSourceLine, CLogDispatcherImpl* p = nullptr)
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

#ifdef ALLOW_SINGLTONE_LOG_DISPATCH
        LogDispatcherSingltone::instance().FireLogMessage(GCN_LL_FUNC, szMessage);
#else
        spLogDispatcher = p;
        if (spLogDispatcher) static_cast<CLogDispatcherImpl*>(spLogDispatcher)->FireLogMessage(GCN_LL_FUNC, szMessage);
#endif
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

#ifdef ALLOW_SINGLTONE_LOG_DISPATCH
        LogDispatcherSingltone::instance().FireLogMessage(GCN_LL_FUNC, szMessage);
#else
        if (spLogDispatcher) static_cast<CLogDispatcherImpl*>(spLogDispatcher)->FireLogMessage(GCN_LL_FUNC, szMessage);
#endif

    }

private:
    std::string m_szSourceFile;
    std::string m_szFunctionName;

    int m_nSourceLine;

#ifndef ALLOW_SINGLTONE_LOG_DISPATCH
    LogDispatcherPtr_t spLogDispatcher;
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

#ifdef ALLOW_SINGLTONE_LOG_DISPATCH
    #define DUMP_FUNCTION() gcn::DumpFunction DumpFunc( __FILE__, __FUNCTION__, __LINE__ )
    #define DUMP_FUNCTION_TO(x) gcn::DumpFunction DumpFunc(__FILE__, __FUNCTION__, __LINE__)
#else
    #define DUMP_FUNCTION()
    #define DUMP_FUNCTION_TO(x) gcn::DumpFunction DumpFunc(__FILE__, __FUNCTION__, __LINE__, static_cast<gcn::CLogDispatcherImpl*>((gcn::ILogDispatcher*)x))
#endif

/*
#define DUMP_EXCEPTION( ex ) {                                                              \
    std::string sMessage( "ThreadID: " );                                                   \
    sMessage.append( boost::lexical_cast< std::string >( boost::this_thread::get_id() ) );  \
    sMessage.append( ": " );                                                                \
    sMessage.append( ex.what() );                                                           \
    LogDispatcherSingltone::instance().FireLogMessage( CC_LL_ERROR, sMessage );             \
}

#define DUMP_BOOST_EXCEPTION( ex ) {                                                        \
    std::string sMessage( "ThreadID: " );                                                   \
    sMessage.append( boost::lexical_cast< std::string >( boost::this_thread::get_id() ) );  \
    sMessage.append( ": " );                                                                \
    sMessage.append( boost::diagnostic_information_what( ex ) );                            \
    LogDispatcherSingltone::instance().FireLogMessage( CC_LL_ERROR, sMessage );             \
}

#define DUMP_MESSAGE( ll, message ) {                                                           \
    if( ll <= LogDispatcherSingltone::instance().GetLogLevel() )                                \
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
    DUMP_MESSAGE( GCN_LL_INFO, message )

#define DUMP_WARNING( message ) \
    DUMP_MESSAGE( GCN_LL_WARNING, message )

#define DUMP_ERROR( message ) \
    DUMP_MESSAGE( GCN_LL_ERROR, message )

#define DUMP_DEBUG( message ) \
    DUMP_MESSAGE( GCN_LL_DEBUG, message )
*/



#endif //__GCN_DUMP_FUNCTION_HPP__
