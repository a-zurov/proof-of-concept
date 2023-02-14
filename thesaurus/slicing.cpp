// slicing.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------
// Slicing with arguments

#include <iostream>
#include <cstring>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#define __DELIM__ '\\'
#else
#define __DELIM__ '/'
#endif
#define __FILENAME__ ( std::strrchr( "/" __FILE__, __DELIM__ ) + 1 )
#define __MAKE_DUMP__ __FILENAME__ << " : " << __LINE__ << " | " << __PRETTY_FUNCTION__

#define LBRACE__ "\n {\n"
#define __RBRACE "\n }\n"

#define cout_dump() std::cout << __MAKE_DUMP__ << '\n'
#define cout_dump_msg(x) std::cout << __MAKE_DUMP__ << ' ' << x << '\n'

#define __VASSOP__ // suppress slicing (due to non-polymorph implicit assignment)

#ifdef __VASSOP__
struct Derived; // ?
#endif

struct Base {

    int b_;

    virtual ~Base() {}

    Base(int j) : b_(j) {
        cout_dump_msg(b_);
    }

#ifdef __VASSOP__
    Base& operator= (const Base& rhs) {
        if (this == &rhs) {
            return *this;
        }
        b_ = rhs.b_;
        cout_dump_msg(b_);
        return *this;
    }

    virtual Base& operator= (const Derived& rhs) {
        cout_dump_msg(b_);
        // return operator= (*static_cast<const Base*>(& rhs));
        return operator= ((const Base&)rhs); // ?
    }
#endif //__VASSOP__

    virtual void dump(std::ostream& os) const {
        os << __MAKE_DUMP__ << "\n Base dump: " << b_;
    }

    virtual Base* clone() const {
        cout_dump();
        return new Base(b_);
    }
};

struct Derived : public Base {

    int d_;

    Derived(int j) : Base(j/2), d_(j) {
        cout_dump_msg(b_ << ' ' << d_);
    }

#ifdef __VASSOP__
    Derived& operator= (const Derived& rhs) override {
        if (this == &rhs) {
            return *this;
        }
        b_ = rhs.b_;
        d_ = rhs.d_;
        cout_dump_msg(b_ << ' ' << d_);
        return *this;
    }
#endif //__VASSOP__

    void dump(std::ostream& os) const override {
        os << __MAKE_DUMP__ << "\n Derived dump: " << b_ << " " << d_;
    }

    Derived* clone() const override {
        cout_dump();
        return new Derived(d_, b_);
    }

protected:
    Derived(int d, int b) : Base(b), d_(d) {
        cout_dump_msg(b_ << ' ' << d_);
    }
};

std::ostream& operator<< (std::ostream & os, const Base & base) {
    //cout_dump();
    base.dump(os);
    return os;
}

void foo(Base base) {
    std::cout << __MAKE_DUMP__
        << LBRACE__ << base << __RBRACE;
}

void bar(Base & base) {
    std::cout << __MAKE_DUMP__
        << LBRACE__ << base << __RBRACE;
}

void par(Base* ptr) {
    std::cout << __MAKE_DUMP__
        << LBRACE__ << *ptr << __RBRACE;
}

int main() {

    Derived d(12);
    foo(d); // slicing due to non-polymorph copy-init
    bar(d);
    par(&d);

    Derived d1(10);
    Derived d2(8);
    Base& ref_b = d1;
    std::cout << ref_b << '\n';
    ref_b = d2; // slicing due to non-polymorph implicit assignment
    std::cout << ref_b << '\n';

    Base* ptr_b = ref_b.clone();
    par(ptr_b);
    delete ptr_b;
}
