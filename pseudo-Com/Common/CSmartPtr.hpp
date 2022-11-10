//////////////////////////////////////////////////////////////////////////
// Module: CSmartPtr.hpp
//////////////////////////////////////////////////////////////////////////

#ifndef __GCN_CSMART_PTR_HPP__
#define __GCN_CSMART_PTR_HPP__

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////

template< typename T >
CSmartPtr< T >::CSmartPtr()
{
    cout_dump_this();

    m_pT = nullptr;
}

template< typename T >
CSmartPtr< T >::CSmartPtr(T* pT)
{
    cout_dump_this();

    m_pT = pT;

    if (nullptr != m_pT)
    {
        m_pT->AddRef();
    }
}

template< typename T >
CSmartPtr< T >::CSmartPtr(const CSmartPtr< T >& spT)
{
    cout_dump_this();

    m_pT = spT.m_pT;

    if (nullptr != m_pT)
    {
        m_pT->AddRef();
    }
}

template< typename T >
CSmartPtr< T >::~CSmartPtr()
{
    cout_dump_this();

    if (nullptr != m_pT)
    {
        m_pT->Release();
    }
}

template< typename T >
const CSmartPtr< T >& CSmartPtr< T >::operator= (T* pT)
{
    cout_dump();

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
    cout_dump();

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
    cout_dump();

    return m_pT;
}

template< typename T >
T& CSmartPtr< T >::operator* () const
{
    cout_dump();

    return *m_pT;
}

template< typename T >
T** CSmartPtr< T >::operator& ()
{
    cout_dump();

    return &m_pT;
}

template< typename T >
bool CSmartPtr< T >::operator! () const
{
    cout_dump();

    return (nullptr == m_pT);
}

template< typename T >
bool CSmartPtr< T >::operator< (T* pT) const
{
    cout_dump();

    return (m_pT < pT);
}

template< typename T >
bool CSmartPtr< T >::operator== (T* pT) const
{
    cout_dump();

    return (m_pT == pT);
}

template< typename T >
bool CSmartPtr< T >::operator!= (T* pT) const
{
    cout_dump();

    return !(operator== (pT));
}

template< typename T >
_NoAddRefReleaseOnCPtr<T>* CSmartPtr<T>::operator-> () const
{
    return static_cast< _NoAddRefReleaseOnCPtr<T>* >(m_pT);
}

template< typename T >
void CSmartPtr< T >::Attach(T* pT)
{
    cout_dump();

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
    cout_dump();

    T* pT = m_pT;
    m_pT = nullptr;

    return pT;
}

template< typename T >
void CSmartPtr< T >::Release()
{
    cout_dump();

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
