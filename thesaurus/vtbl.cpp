// vtbl.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------

#include <iostream>
#include <cstring>
#include <cassert>

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

struct A {

    int a_{};

    void foo(void) {
        cout_dump_msg(this);
    }

    void bar(void) {
        cout_dump_msg(this);
    }
};

struct B : public A {

    int b_{};

    void foo(void) {
        cout_dump_msg(this);
    }
};


int main() {

    A a1;
    a1.foo();
    cout_dump_msg(&a1);
    cout_dump_msg(&a1.a_);

#ifdef _MSC_VER
    assert((sizeof(decltype(&A::foo)) == sizeof(void*)));
#else
    cout_dump_msg((void*)&A::foo);
    cout_dump_msg((void*)&A::bar);
    cout_dump_msg((void*)&B::bar);
    assert(((void*)&A::bar == (void*)&B::bar));
#endif

    B b1;
    b1.foo();
    b1.bar();
    cout_dump_msg(&b1.a_);
    cout_dump_msg(&b1.b_);
}
