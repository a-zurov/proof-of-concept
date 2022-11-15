//////////////////////////////////////////////////////////////////////////
// File name: SignalSlot.h
//////////////////////////////////////////////////////////////////////////

#ifndef __XCOM_SIGNAL_SLOT_H__
#define __XCOM_SIGNAL_SLOT_H__

#include <list>

#include "Types.h"
#include "DumpFunction.hpp"

//////////////////////////////////////////////////////////////////////////

XCOM_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////

template<typename R> class Function;

template<typename R, typename... Args>
class Function<R(Args...)>
{
    void* m_this_ptr;
    R(*m_stub_ptr)(void*, Args...);

    template<typename I, typename F>
    Function(I&& this_ptr, F&& stub_ptr)
        : m_this_ptr( std::forward<I>(this_ptr) )
        , m_stub_ptr( std::forward<F>(stub_ptr) )
    {
        PREBIND_DUMP_FUNCTION();
    }

public:

    template<R(*func_ptr)(Args...)>
    static inline Function bind()
    {
        PREBIND_DUMP_FUNCTION();

        return  { nullptr
                , [](void*, Args... args) {
                        return (*func_ptr)(args...);
                    }
                };
    }

    template<typename T, R(T::* method_ptr)(Args...)>
    static inline Function bind(T* pointer)
    {
        PREBIND_DUMP_FUNCTION();

        return  { pointer
                , [](void* this_ptr, Args... args) {
                        return (static_cast<T*>(this_ptr)->*method_ptr)(args...);
                    }
                };
    }

    template<typename T, R(T::* method_ptr)(Args...) const>
    static inline Function bind(T* pointer)
    {
        PREBIND_DUMP_FUNCTION();

        return  { pointer
                , [](void* this_ptr, Args... args) {
                        return (static_cast<const T*>(this_ptr)->*method_ptr)(args...);
                }
        };
    }

    R operator() (Args&&... args) const
    {
        PREBIND_DUMP_FUNCTION();

        return (*m_stub_ptr)(m_this_ptr, args...);
    }
};

template <typename R> class SignalEmitter;

template <typename R, typename... Args>
class SignalEmitter<R(Args...)>
{
    typedef xcom::Function<R(Args...)> Function;

    std::list<Function> m_listSlots;

public:

    template <R(*func_ptr)(Args...)>
    void connect()
    {
        PREBIND_DUMP_FUNCTION();

        m_listSlots.emplace_back(Function::template bind<func_ptr>());
    }

    template <typename T, R(T::* method_ptr)(Args...)>
    void connect(T* instance)
    {
        PREBIND_DUMP_FUNCTION();

        m_listSlots.emplace_back(Function::template bind<T, method_ptr>(instance));
    }

    template <typename T, R(T::* method_ptr)(Args...) const>
    void connect(T* instance)
    {
        PREBIND_DUMP_FUNCTION();

        m_listSlots.emplace_back(Function::template bind<T, method_ptr>(instance));
    }

    void disconnect()
    {
        m_listSlots.clear();
    }

    void operator()(Args... var) const
    {
        PREBIND_DUMP_FUNCTION();

        for (auto const& slot : m_listSlots) {
            slot(std::forward<Args>(var)...);
        }
    }
};

//////////////////////////////////////////////////////////////////////////

XCOM_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif //__XCOM_SIGNAL_SLOT_H__

