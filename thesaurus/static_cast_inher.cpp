// static_cast.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -----------------------------------------------
// Static casts with statically calculated offsets

#include <iostream>
#include <cstddef>

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

struct Base {
    int a_;
    Base(int a = 0) : a_{a} {
        cout_dump_msg("sizeof = " << sizeof(Base));
        cout_dump_msg("this = " << this);
        cout_dump_msg("offsetof(a_) = " << offsetof(Base, a_));
        cout_dump_msg("&a_  = " << &a_);
    }
    virtual void fill() = 0;
    virtual ~Base() {
        cout_dump();
    }
};

struct InputBase {
    int b_;
    InputBase(int b) : b_{b} {
        cout_dump_msg("sizeof = " << sizeof(InputBase));
        cout_dump_msg("this = " << this);
        cout_dump_msg("offsetof(b_) = " << offsetof(InputBase, b_));
        cout_dump_msg("&b_  = " << &b_);
    }
    virtual ~InputBase() {
        cout_dump();
    }
};

struct OutputBase {
    int c_;
    OutputBase(int c) : c_{c} {
        cout_dump_msg("sizeof = " << sizeof(OutputBase));
        cout_dump_msg("this = " << this);
        cout_dump_msg("offsetof(c_) = " << offsetof(OutputBase, c_));
        cout_dump_msg("&c_  = " << &c_);
    }
    virtual ~OutputBase() {
        cout_dump();
    }
};

struct IODerived final : Base, InputBase, OutputBase {
    int d_;
    void fill() override {
        cout_dump();
    }
    IODerived(int d) : InputBase(d*2), OutputBase(d*3), d_{d} {
        cout_dump_msg("sizeof = " << sizeof(IODerived));
        cout_dump_msg("this = " << this);
        cout_dump_msg("offsetof(d_) = " << offsetof(IODerived, d_));
        cout_dump_msg("&d_  = " << &d_);
    }
};

int main() {

    IODerived* pIOD = new IODerived{ 5 };

    cout_dump_msg(std::hex << "pIOD = " << pIOD << ": " << std::dec << pIOD->d_);

    {
        OutputBase* pOB = static_cast<OutputBase*>(pIOD);

        cout_dump_msg(std::hex << "pOB = " << pOB << ": " << std::dec << pOB->c_);

        pIOD = static_cast<IODerived*>(pOB);

        cout_dump_msg(std::hex << "pIOD = " << pIOD << ": " << std::dec << pIOD->d_);
    }

    {
        InputBase* pIB = static_cast<InputBase*>(pIOD);

        cout_dump_msg(std::hex << "pIB = " << pIB << ": " << std::dec << pIB->b_);

        pIOD = static_cast<IODerived*>(pIB);

        cout_dump_msg(std::hex << "pIOD = " << pIOD << ": " << std::dec << pIOD->d_);
    }

    Base* pB = static_cast<Base*>(pIOD);
    pB->fill();
    delete pB;
}
