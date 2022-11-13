//////////////////////////////////////////////////////////////////////////
// Module: CSmartPtr.hpp
//////////////////////////////////////////////////////////////////////////

#ifndef __GCN_CSMART_PTR_HPP__
#define __GCN_CSMART_PTR_HPP__

#include "CLogDispatcherImpl.h"

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////

template< typename T >
CSmartPtr< T >::CSmartPtr()
{
    DUMP_FUNCTION();

    m_pT = nullptr;
}

template< typename T >
CSmartPtr< T >::CSmartPtr(T* pT)
{
    DUMP_FUNCTION();

    m_pT = pT;

    if (nullptr != m_pT)
    {
        m_pT->AddRef();
    }
}

template< typename T >
CSmartPtr< T >::CSmartPtr(const CSmartPtr< T >& spT)
{
    DUMP_FUNCTION();

    m_pT = spT.m_pT;

    if (nullptr != m_pT)
    {
        m_pT->AddRef();
    }
}

template< typename T >
CSmartPtr< T >::~CSmartPtr()
{
    DUMP_FUNCTION();

    if (nullptr != m_pT)
    {
        m_pT->Release();
    }
}

template< typename T >
const CSmartPtr< T >& CSmartPtr< T >::operator= (T* pT)
{
    DUMP_FUNCTION();

    if (m_pT != pT)
    {
        if (nullptr != m_pT)
        {
            m_pT->Release();
        }

        if (nullptr != pT)
        {
            pT->AddRef();
        }

        m_pT = pT;
    }

    return *this;
}

template< typename T >
const CSmartPtr< T >& CSmartPtr< T >::operator= (const CSmartPtr< T >& spT)
{
    DUMP_FUNCTION();

    if (this != &spT && m_pT != spT.m_pT)
    {
        if (nullptr != m_pT)
        {
            m_pT->Release();
        }

        if (nullptr != spT.m_pT)
        {
            spT.m_pT->AddRef();
        }

        m_pT = spT.m_pT;
    }

    return *this;
}

template< typename T >
CSmartPtr< T >::operator T* () const
{
    DUMP_FUNCTION();

    return m_pT;
}

template< typename T >
T& CSmartPtr< T >::operator* () const
{
    DUMP_FUNCTION();

    return *m_pT;
}

template< typename T >
T** CSmartPtr< T >::operator& ()
{
    DUMP_FUNCTION();

    return &m_pT;
}

template< typename T >
bool CSmartPtr< T >::operator! () const
{
    DUMP_FUNCTION();

    return (nullptr == m_pT);
}

template< typename T >
bool CSmartPtr< T >::operator< (T* pT) const
{
    DUMP_FUNCTION();

    return (m_pT < pT);
}

template< typename T >
bool CSmartPtr< T >::operator== (T* pT) const
{
    DUMP_FUNCTION();

    return (m_pT == pT);
}

template< typename T >
bool CSmartPtr< T >::operator!= (T* pT) const
{
    DUMP_FUNCTION();

    return !(operator== (pT));
}

template< typename T >
_NoAddRefReleaseOnCPtr<T>* CSmartPtr<T>::operator-> () const
{
    return static_cast<_NoAddRefReleaseOnCPtr<T>*>(m_pT);
}

template< typename T >
void CSmartPtr< T >::Attach(T* pT)
{
    DUMP_FUNCTION();

    if (m_pT != pT)
    {
        if (nullptr != m_pT)
        {
            m_pT->Release();
        }

        m_pT = pT;
    }
}

template< typename T >
T* CSmartPtr< T >::Detach()
{
    DUMP_FUNCTION();

    T* pT = m_pT;
    m_pT = nullptr;

    return pT;
}

template< typename T >
void CSmartPtr< T >::Release()
{
    DUMP_FUNCTION();

    T* pT = m_pT;

    if (nullptr != pT)
    {
        m_pT = nullptr;
        pT->Release();
    }
}

template< typename T >class _NoAddRefReleaseOnCPtr : public T
{
private:
    long AddRef() = 0;
    long Release() = 0;
};

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif //__CC_CSMART_PTR_HPP__
