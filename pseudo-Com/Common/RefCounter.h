//////////////////////////////////////////////////////////////////////////
// Module: RefCounter.h - managing lifetimes through reference counting
//////////////////////////////////////////////////////////////////////////

#ifndef __GCN_CREF_COUNTER_H__
#define __GCN_CREF_COUNTER_H__

//////////////////////////////////////////////////////////////////////////

#include "Types.h"

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////

class CRefCounter
{
public:
    virtual ~CRefCounter();

    CRefCounter();

    long AddRef();

    long Release();

    long get();

protected:
    virtual void Close() = 0;

private:
    void SelfDestroy()
    {
        delete this;
    }

private:
    volatile UINT32_t m_nRef;
};

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif //__GCN_CREF_COUNTER_H__
