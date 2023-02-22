// static_cast.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -----------------------------------------------
// Static casts with statically calculated offsets

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

#define cout_dump_lock() { std::lock_guard<std::mutex> lock(g_mutex); \
                         std::cout << __MAKE_DUMP__ << '\n'; }

//#define _DUMP_EX

#if defined (_DUMP_EX )
#define cout_dump_lock_ex() cout_dump_lock()
#else
#define cout_dump_lock_ex()
#endif

struct Base {

    static std::vector<std::thread> vec_threads_;

    virtual void operator ()() = 0;
    virtual ~Base() {
        cout_dump_lock_ex();
    }
};

std::vector<std::thread> Base::vec_threads_;

struct InputBase :virtual Base {

    InputBase() {
        vec_threads_.emplace_back(*this);
    }
    void operator ()() override {
        cout_dump_lock();
    }
    virtual ~InputBase() {
        cout_dump_lock_ex();
    }
};

struct OutputBase : virtual Base {

    OutputBase() {
        vec_threads_.emplace_back(*this);
    }
    void operator ()() override {
        cout_dump_lock();
    }
    virtual ~OutputBase() {
        cout_dump_lock_ex();
    }
};

struct IODerived final : InputBase, OutputBase {

    IODerived() : InputBase(), OutputBase() {

        vec_threads_.emplace_back(*this);
        vec_threads_.emplace_back(*static_cast<InputBase*>(this));
        vec_threads_.emplace_back(*static_cast<OutputBase*>(this));

#if defined (_DUMP_EX )
        static_cast<InputBase*>(this)->operator()();
        static_cast<OutputBase*>(this)->operator()();
#endif
    }
    void operator ()() override {
        cout_dump_lock();
    }
    virtual ~IODerived() {
        cout_dump_lock_ex();
    }
};

int main() {

    IODerived d;

    for (auto& th : Base::vec_threads_) {
        th.join();
    }
}
