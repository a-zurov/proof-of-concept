//////////////////////////////////////////////////////////////////////////
// Module: CSmartPtr.h can ++/-- reference counter and destroy (when zero)
// classes derived from CRefCounter & IBase
//////////////////////////////////////////////////////////////////////////

#ifndef __GCN_CSMART_PTR_H__
#define __GCN_CSMART_PTR_H__

//////////////////////////////////////////////////////////////////////////

#include "Types.h"

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////

template< typename T >
class _NoAddRefReleaseOnCPtr;

template< typename T >
class CSmartPtr
{
public:
    virtual ~CSmartPtr();

    CSmartPtr();

    CSmartPtr(T* pT);

    CSmartPtr(const CSmartPtr<T>& spT);

    const CSmartPtr<T>& operator= (T* pT);

    const CSmartPtr<T>& operator= (const CSmartPtr<T>& spT);

    operator T* () const;

    T& operator* () const;

    T** operator& ();

    bool operator! () const;

    bool operator< (T* pT) const;

    bool operator== (T* pT) const;

    bool operator!= (T* pT) const;

    _NoAddRefReleaseOnCPtr<T>* operator-> () const;

    void Attach(T* pT);

    T* Detach();

    void Release();

private:
    T* m_pT;
};

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#include "CSmartPtr.hpp"

//////////////////////////////////////////////////////////////////////////

#endif //__GCN_CSMART_PTR_H__