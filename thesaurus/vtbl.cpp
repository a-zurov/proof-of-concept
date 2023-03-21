// vtbl.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------

#include <iostream>
#include <cstring>
#include <cassert>
#include <functional>

#ifdef _MSC_VER
#define __CXX_VER__ _MSVC_LANG
#define __PRETTY_FUNCTION__ __FUNCSIG__
#define __DELIM__ '\\'
#else
#define __CXX_VER__ __cplusplus
#define __DELIM__ '/'
#endif

#define Cxx_20__ 202002L

#if ( Cxx_20__ <= __CXX_VER__ )
#include <array>
#include <bit>
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

struct A {

    int a_{};

    virtual void foo(void) {
        cout_dump_msg("this = " << this);
    }

    void bar(void) {
        cout_dump_msg("this = " << this);
    }
};

struct B : public A {

    int b_{};

    void foo(void) override {
        cout_dump_msg("this = " << this);
    }
};

int main() {

    A a;
    auto method = &A::foo;
    std::invoke(method, &a);
#if ( Cxx_20__ <= __CXX_VER__ )
    using bytearray = std::array<std::byte, sizeof(method)>;
    auto const& casted = std::bit_cast<bytearray, decltype(method)>(method);
    std::cout << "0x" << std::hex;
    for (auto b : casted)
        std::cout << std::to_integer<int>(b) << ' ';
    std::cout << '\n';
#endif

#ifndef _MSC_VER
    cout_dump_msg((void*)&A::foo);
    cout_dump_msg((void*)&B::foo);
#endif

    cout_dump_msg(void_cast(&A::bar));
    assert(void_cast(&A::bar) == void_cast(&B::bar));

    A a1, a2;
    a1.foo();
    cout_dump_msg(&a1);
    cout_dump_msg(&a1.a_);

    cout_dump_msg(*(void**)&a1);
    assert(*(void**)&a1 == *(void**)&a2);

#ifndef _MSC_VER
    assert(((void*)&A::foo == *(void**)*(void**)&a1));
    assert(((void*)&A::foo == *(void**)*(void**)&a2));
#endif

    B b1, b2;
    b1.foo();
    cout_dump_msg(&b1);
    cout_dump_msg(&b1.a_);
    cout_dump_msg(&b1.b_);

    cout_dump_msg(*(void**)&b1);
    assert(*(void**)&b1 == *(void**)&b2);

#ifndef _MSC_VER
    assert(((void*)&B::foo == *(void**)*(void**)&b1));
    assert(((void*)&B::foo == *(void**)*(void**)&b2));
#endif
}
