//////////////////////////////////////////////////////////////////////////
// Module: IBase.h - The IUnknown interface for Component Object Model
//////////////////////////////////////////////////////////////////////////

#ifndef __GCN_I_BASE_H__
#define __GCN_I_BASE_H__

//////////////////////////////////////////////////////////////////////////

#include "Types.h"

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////

class IBase
{
public:
    virtual ~IBase() {}

    virtual long AddRef() = 0;

    virtual long Release() = 0;

    virtual ResultCode QueryInterface(const GCN_UUID& uuid, void** ppInterface)
    {
        return GCN_UNKNOWN_INTERFACE;
    }
};

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif //__GCN_I_BASE_H__
