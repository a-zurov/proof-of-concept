// Common.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"


GCN_NAMESPACE_BEGIN

// TODO: This is an example of a library function
void fnCommon()
{
}

ILogger* GetLogger()
{
    return static_cast<ILogger*>(new CLoggerImpl());
}

GCN_NAMESPACE_END
