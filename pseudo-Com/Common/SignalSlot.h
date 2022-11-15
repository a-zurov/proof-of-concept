//////////////////////////////////////////////////////////////////////////
// File name: SignalSlot.h
//////////////////////////////////////////////////////////////////////////

#ifndef __XCOM_SIGNAL_SLOT_H__
#define __XCOM_SIGNAL_SLOT_H__

#include "Types.h"
#include <list>

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
    Function(I&& this_ptr, F&& stub_ptr) :
        m_this_ptr(std::forward<I>(this_ptr)),
        m_stub_ptr(std::forward<F>(stub_ptr)) {}

public:

    template<R(*func_ptr)(Args...)>
    static inline Function bind()
    {
        return { nullptr, [](void*, Args... args) {
            return (*func_ptr)(args...); }
        };
    }

    template<typename T, R(T::* method_ptr)(Args...)>
    static inline Function bind(T* pointer)
    {
        return { pointer, [](void* this_ptr, Args... args) {
            return (static_cast<T*>(this_ptr)->*method_ptr)(args...); }
        };
    }

    template<typename T, R(T::* method_ptr)(Args...) const>
    static inline Function bind(T* pointer)
    {
        return { pointer, [](void* this_ptr, Args... args) {
            return (static_cast<const T*>(this_ptr)->*method_ptr)(args...); }
        };
    }

    R operator() (Args&&... args) const
    {
        return (*m_stub_ptr)(m_this_ptr, args...);
    }
};


template <typename R> class Signal;
template <typename R, typename... Args>
class Signal<R(Args...)>
{
    typedef xcom::Function<R(Args...)> Function;
    std::list<Function> slots;

public:

    template <R(*func_ptr)(Args...)>
    void connect()
    {
        auto fn = Function::template bind<func_ptr>();
        slots.push_back(fn);
    }

    template <typename T, R(T::* meth_ptr)(Args...)>
    void connect(T* instance)
    {
        auto fn = Function::template bind<T, meth_ptr>(instance);
        slots.push_back(fn);
    }

    template <typename T, R(T::* meth_ptr)(Args...) const>
    void connect(T* instance)
    {
        auto fn = Function::template bind<T, meth_ptr>(instance);
        slots.push_back(fn);
    }

    void disconnect()
    {
        slots.clear();
    }

    void operator()(Args... var) const
    {
        for (auto const& slot : slots) {
            slot(std::forward<Args>(var)...);
        }
    }
};

//////////////////////////////////////////////////////////////////////////

XCOM_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif //__XCOM_SIGNAL_SLOT_H__

