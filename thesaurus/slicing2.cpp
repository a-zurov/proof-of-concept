// slicing2.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
// Static casts leads to slicing but it's not a polymorphic behaviour

#include <iostream>
#include <cstddef>
#include <cassert>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

#if defined( _MSC_VER )
#define __PRETTY_FUNCTION__ __FUNCSIG__
#define __DELIM__ '\\'
#else
#define __DELIM__ '/'
#endif
#define __FILENAME__ ( std::strrchr( "/" __FILE__, __DELIM__ ) + 1 )
#define __MAKE_DUMP__ __FILENAME__ << " : " << __LINE__ << " | " << __PRETTY_FUNCTION__

#define cout_dump() std::cout << __MAKE_DUMP__ << '\n'
#define cout_dump_msg(x) std::cout << __MAKE_DUMP__ << ' ' << x << '\n'

std::mutex g_mutex;

#define cout_dump_lock()        { std::lock_guard<std::mutex> lock(g_mutex); \
                                    cout_dump(); }
#define cout_dump_msg_lock(x)   { std::lock_guard<std::mutex> lock(g_mutex); \
                                    cout_dump_msg(x); }

//#define _DUMP_EX

#if defined (_DUMP_EX )
#define cout_dump_lock_ex()         cout_dump_lock()
#define cout_dump_msg_lock_ex(x)    cout_dump_msg_lock(x)
#else
#define cout_dump_lock_ex()
#define cout_dump_msg_lock_ex(x)
#endif

struct Base {

    static std::vector<std::thread> vec_threads_;

    virtual void foo() = 0;
    virtual void operator ()(Base*) = 0;
    virtual ~Base() {
        cout_dump_lock_ex();
    }
};

std::vector<std::thread> Base::vec_threads_;

struct InputBase : virtual Base {

    int b_{};

    InputBase() {
        cout_dump_msg_lock("this = " << std::hex << this);
        foo();
        vec_threads_.emplace_back( std::thread{ *this, this } );
    }
    InputBase(const InputBase& ib) {
        cout_dump_msg_lock("this = " << std::hex << this << " from " << &ib);
    }
    void foo() override {
        cout_dump_msg_lock("this = " << std::hex << this);
    }
    void operator ()(Base* p) override {
        cout_dump_msg_lock("this = " << std::hex << this << " Base* p = " << p);
        foo();
        p->foo();
    }
    virtual ~InputBase() {
        cout_dump_msg_lock("this = " << std::hex << this);
    }
};

struct OutputBase : virtual Base {

    int c_{};

    OutputBase() {
        cout_dump_msg_lock_ex("this = " << std::hex << this);
        vec_threads_.emplace_back( std::thread{ *this, this } );
    }
    OutputBase(const OutputBase& ob) {
        cout_dump_msg_lock_ex("this = " << std::hex << this << " from " << &ob);
    }
    void foo() override {
        cout_dump_lock_ex();
    }
    void operator ()(Base* p) override {
        cout_dump_msg_lock_ex("this = " << std::hex << this << " Base* p = " << p);
        foo();
        p->foo();
    }
    virtual ~OutputBase() {
        cout_dump_lock_ex();
    }
};

struct IODerived final : InputBase, OutputBase {

    int d_{};

    IODerived() : InputBase(), OutputBase() {

        std::this_thread::sleep_for(std::chrono::seconds(1));

        cout_dump_msg_lock("this = " << std::hex << this);

        vec_threads_.emplace_back( std::thread{ *this, this } );

        //vec_threads_.emplace_back( std::thread{ *static_cast<InputBase*>(this), this } );
        //vec_threads_.emplace_back( std::thread{ *static_cast<OutputBase*>(this), this } );
        //static_cast<InputBase*>(this)->operator()(this);
        //static_cast<OutputBase*>(this)->operator()(this);
    }
    IODerived(const IODerived& ob) {
        cout_dump_msg_lock("this = " << std::hex << this << " from " << &ob);
    }
    void foo() override {
        cout_dump_msg_lock("this = " << std::hex << this);
    }
    void operator ()(Base* p) override {
        cout_dump_msg_lock("this = " << std::hex << this << " Base* p = " << p);
        foo();
        p->foo();
    }
    virtual ~IODerived() {
        cout_dump_lock();
    }
};

int main() {

    cout_dump_msg(sizeof(InputBase));
    cout_dump_msg(offsetof(InputBase, b_));
    cout_dump_msg(alignof(InputBase));

    cout_dump_msg(sizeof(IODerived));
    cout_dump_msg(offsetof(IODerived, d_));
    cout_dump_msg(alignof(IODerived));

    IODerived d;

    for (auto& th : Base::vec_threads_) {
        th.join();
    }

#ifndef _MSC_VER
    assert(((void*)&IODerived::foo == *(void**)*(void**)&d));
#endif

    //std::thread th{ &IODerived::foo, &d };
    //th.join();
}
