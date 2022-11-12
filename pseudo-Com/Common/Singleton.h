//////////////////////////////////////////////////////////////////////////
// File name: Singleton.h
//////////////////////////////////////////////////////////////////////////

#ifndef __CC_SINGLETON_H__
#define __CC_SINGLETON_H__

#include "Types.h"

GCN_NAMESPACE_BEGIN

template < typename T >
class singleton : public T
{
public:
    static singleton< T >& instance()
    {
        static singleton< T > obj;
        create_object.do_nothing();
        return obj;
    }

private:
    class object_creator
    {
    public:
        object_creator() { singleton< T >::instance(); }
        inline void do_nothing() const { }
    };
    static object_creator create_object;

    singleton() {}
    singleton(const singleton&);
    singleton& operator= (const singleton&);
};

template < typename T > typename singleton< T >::object_creator singleton< T >::create_object;

GCN_NAMESPACE_END

#endif //__CC_SINGLETON_H__
