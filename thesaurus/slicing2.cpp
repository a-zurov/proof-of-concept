// slicing2.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------
// Static casts leads to slicing but it's not a polymorphic behaviour

#include <iostream>
#include <cstddef>
#include <vector>
#include <thread>
#include <mutex>

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

    InputBase() {
        cout_dump_msg_lock("this = " << std::hex << this);
        vec_threads_.emplace_back( std::thread{ *this, this } );
    }
    InputBase(const InputBase& ib) {
        cout_dump_msg_lock("this = " << std::hex << this << ' ' << &ib);
    }
    void foo() override {
        cout_dump_lock();
    }
    void operator ()(Base* p) override {
        cout_dump_msg_lock("this = " << std::hex << this << ' ' << p);
        p->foo();
        foo();
    }
    virtual ~InputBase() {
        cout_dump_lock();
    }
};

struct OutputBase : virtual Base {

    OutputBase() {
        cout_dump_msg_lock_ex("this = " << std::hex << this);
        vec_threads_.emplace_back( std::thread{ *this, this } );
    }
    OutputBase(const OutputBase& ob) {
        cout_dump_msg_lock_ex("this = " << std::hex << this << ' ' << &ob);
    }
    void foo() override {
        cout_dump_lock_ex();
    }
    void operator ()(Base* p) override {
        cout_dump_msg_lock_ex("this = " << std::hex << this << ' ' << p);
        p->foo();
        foo();
    }
    virtual ~OutputBase() {
        cout_dump_lock_ex();
    }
};

struct IODerived final : InputBase, OutputBase {

    IODerived() : InputBase(), OutputBase() {

        vec_threads_.emplace_back( std::thread{ *this, this } );

        //vec_threads_.emplace_back( std::thread{ *static_cast<InputBase*>(this), this } );
        //vec_threads_.emplace_back( std::thread{ *static_cast<OutputBase*>(this), this } );
        //static_cast<InputBase*>(this)->operator()(this);
        //static_cast<OutputBase*>(this)->operator()(this);

    }
    IODerived(const IODerived& ob) {
        cout_dump_msg_lock("this = " << std::hex << this << ' ' << &ob);
    }
    void foo() override {
        cout_dump_msg_lock("this = " << std::hex << this);
    }
    void operator ()(Base* p) override {
        cout_dump_msg_lock("this = " << std::hex << this << ' ' << p);
        p->foo();
        foo();
    }
    virtual ~IODerived() {
        cout_dump_lock();
    }
};

int main() {

    IODerived d;

    for (auto& th : Base::vec_threads_) {
        th.join();
    }

    //std::thread th{ &IODerived::foo, &d };
    //th.join();

}
