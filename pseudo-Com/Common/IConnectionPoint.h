//////////////////////////////////////////////////////////////////////////
// Module: IConnectionPoint.h
//////////////////////////////////////////////////////////////////////////

#ifndef __GCN_I_CONNECTION_POINT_H__
#define __GCN_I_CONNECTION_POINT_H__

//////////////////////////////////////////////////////////////////////////

#include "Types.h"
#include "IBase.h"

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////
// {3C66446F-E852-4EA5-85ED-2A7CFBCC7D57}

static const GCN_UUID IConnectionPointContainer_UUID =
{ 0x3c66446f, 0xe852, 0x4ea5, { 0x85, 0xed, 0x2a, 0x7c, 0xfb, 0xcc, 0x7d, 0x57 } };

class IConnectionPointContainer
{
public:
    virtual ~IConnectionPointContainer() {}

    virtual ResultCode Bind(const GCN_UUID& uuid, void* pInterface)
    {
        return GCN_UNKNOWN_INTERFACE;
    }

    virtual ResultCode Unbind(const GCN_UUID& uuid, void* pInterface)
    {
        return GCN_UNKNOWN_INTERFACE;
    }
};

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif //__GCN_I_CONNECTION_POINT_H__
