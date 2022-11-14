//////////////////////////////////////////////////////////////////////////
// Module: AtomicSync.h makes your volatile variables atomic
//////////////////////////////////////////////////////////////////////////

#ifndef __GCN_ATOMIC_SYNC_H__
#define __GCN_ATOMIC_SYNC_H__

//////////////////////////////////////////////////////////////////////////

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/interprocess/detail/atomic.hpp>

#include "Types.h"

//////////////////////////////////////////////////////////////////////////

#define SYNC_NAMESPACE_BEGIN XCOM_NAMESPACE_BEGIN namespace sync {
#define SYNC_NAMESPACE_END } XCOM_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

SYNC_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////

typedef boost::shared_lock< boost::shared_mutex > read_guard_t;
typedef boost::unique_lock< boost::shared_mutex > write_guard_t;
typedef boost::upgrade_lock< boost::shared_mutex > upgrade_guard_t;
typedef boost::upgrade_to_unique_lock< boost::shared_mutex > upto_write_guard_t;

#if ( BOOST_VERSION / 100000 == 1 ) && ( ( BOOST_VERSION / 100 ) % 1000 ) >= 48
namespace bipc = boost::interprocess::ipcdetail;
#else
namespace bipc = boost::interprocess::detail;
#endif //BOOST_VERSION


inline UINT32_t AtomicInc32(volatile UINT32_t& n32Var)
{
    return bipc::atomic_inc32(&n32Var);
}

inline UINT32_t AtomicDec32(volatile UINT32_t& n32Var)
{
    return bipc::atomic_dec32(&n32Var);
}

inline UINT32_t AtomicRead32(volatile UINT32_t& n32Var)
{
    return bipc::atomic_read32(&n32Var);
}

inline void AtomicWrite32(volatile UINT32_t& n32Var, UINT32_t n32Val)
{
    bipc::atomic_write32(&n32Var, n32Val);
}

#ifndef _WIN32

inline UINT32_t AtomicAdd32(volatile UINT32_t& n32Var, UINT32_t n32Val)
{
    return bipc::atomic_add32(&n32Var, n32Val);
}

#endif // _WIN32

inline UINT32_t AtomicCas32(volatile UINT32_t& n32Var, UINT32_t n32With, UINT32_t n32Cmp)
{
    return bipc::atomic_cas32(&n32Var, n32With, n32Cmp);
}

inline UINT64_t AtomicInc64(volatile UINT64_t& n64Var, boost::shared_mutex& mtxLocker)
{
    write_guard_t wLock(mtxLocker);

    return n64Var++;
}

inline UINT64_t AtomicDec64(volatile UINT64_t& n64Var, boost::shared_mutex& mtxLocker)
{
    write_guard_t wLock(mtxLocker);

    return n64Var--;
}

inline UINT64_t AtomicRead64(volatile UINT64_t& n64Var, boost::shared_mutex& mtxLocker)
{
    read_guard_t rLock(mtxLocker);

    return n64Var;
}

inline void AtomicWrite64(volatile UINT64_t& n64Var, UINT64_t ui64Val, boost::shared_mutex& mtxLocker)
{
    write_guard_t wLock(mtxLocker);

    n64Var = ui64Val;
}

inline UINT64_t AtomicAdd64(volatile UINT64_t& n64Var, UINT64_t ui64Val, boost::shared_mutex& mtxLocker)
{
    upgrade_guard_t rLock(mtxLocker);

    volatile UINT64_t ui64OldVal = n64Var;

    upto_write_guard_t wLock(rLock);

    n64Var += ui64Val;

    return ui64OldVal;
}

inline UINT64_t AtomicCas64(volatile UINT64_t& n64Var, UINT64_t ui64With, UINT64_t ui64Cmp, boost::shared_mutex& mtxLocker)
{
    upgrade_guard_t rLock(mtxLocker);

    volatile UINT64_t ui64OldVal = n64Var;

    if (n64Var == ui64Cmp)
    {
        upto_write_guard_t wLock(rLock);

        n64Var = ui64With;
    }

    return ui64OldVal;
}

//////////////////////////////////////////////////////////////////////////

SYNC_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif //__GCN_ATOMIC_SYNC_H__
