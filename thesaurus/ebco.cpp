// ebco.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------

#include <iostream>
#include <cstring>
#include <cassert>
#include <cstddef>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#define __DELIM__ '\\'
#else
#define __DELIM__ '/'
#endif
#define __FILENAME__ ( std::strrchr( "/" __FILE__, __DELIM__ ) + 1 )
#define __MAKE_DUMP__ __FILENAME__ << " : " << __LINE__ << " | " << __PRETTY_FUNCTION__

#define cout_dump() std::cout << __MAKE_DUMP__ << '\n'
#define cout_dump_msg(x) std::cout << __MAKE_DUMP__ << ' ' << x << '\n'

template<   typename T,
            typename R,
            typename = std::is_convertible<R(T::*)(), void*>
        >
void* void_cast(R(T::* f)())
{
#ifdef _MSC_VER
    assert(sizeof(R(T::*)()) == sizeof(void*));
#endif
    union
    {
        R(T::* pf)();
        void* p;
    };
    pf = f;
    return p;
}

struct Empty {

    void ops(void) {
        cout_dump_msg("this = " << this);
    }
};

struct A : Empty {

    void bar(void) {
        cout_dump_msg("this = " << this);
    }
};

struct B : public A {

    int b_{};

    virtual void foo(void) {
        cout_dump_msg("this = " << this);
    }
};

int main() {
    cout_dump_msg(alignof(A));
    cout_dump_msg(sizeof(A));
    cout_dump_msg(alignof(B));
    cout_dump_msg(sizeof(B));
    cout_dump_msg(offsetof(B, b_));

#ifndef _MSC_VER
    cout_dump_msg((void*)&B::foo);
#endif

    cout_dump_msg(void_cast(&A::bar));
    assert(void_cast(&A::bar) == void_cast(&B::bar));
    assert(void_cast(&Empty::ops) == void_cast(&B::ops));

    B b1, b2;
    b1.foo();
    cout_dump_msg(&b1);
    cout_dump_msg(&b1.b_);

    cout_dump_msg(*(void**)&b1);
    assert(*(void**)&b1 == *(void**)&b2);

#ifndef _MSC_VER
    assert(((void*)&B::foo == *(void**)*(void**)&b1));
    assert(((void*)&B::foo == *(void**)*(void**)&b2));
#endif
}
