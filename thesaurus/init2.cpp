// init2.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------

#include <iostream>
#include <cstring>
#include <memory>

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

//#define __MAKE_EXPLICIT__ // <- switch here (!)

#ifdef __MAKE_EXPLICIT__
#define __EXPLICIT__ explicit
#else
#define __EXPLICIT__
#endif

struct Data;

struct Id {
    int m_id;

    Id(int j = 0) : m_id(j) {
        cout_dump_msg(m_id);
    }

    __EXPLICIT__ operator Data() const;

    __EXPLICIT__ operator int() const;
};

struct Data {
    int m_iid;

    ~Data() {
        cout_dump_msg(m_iid);
    }

    explicit Data(Id id) : m_iid(id.m_id) {
        cout_dump_msg(m_iid);
    }

    Data(int j) : m_iid(j) {
        cout_dump_msg(m_iid);
    }

    Data& operator=(const Data& d) {
        m_iid = d.m_iid;
        cout_dump_msg(m_iid);
        return *this;
    }

    Data& operator=(const Id& id) {
        m_iid = id.m_id;
        cout_dump_msg(m_iid);
        return *this;
    }
};

Id::operator Data() const {
    cout_dump();
    return Data(*this);
}

Id::operator int() const {
    cout_dump();
    return this->m_id;
}

int f(const Data& d) {
    cout_dump_msg(d.m_iid);
    return d.m_iid;
}

void g(std::unique_ptr<Data>&& p) {
    cout_dump_msg(p->m_iid);
}

void h(std::shared_ptr<Data> p) {
    cout_dump_msg(p->m_iid);
}

void bar(...) {
    cout_dump();
}

void bar(Id id) {
    cout_dump();
}

void foo(Data d) {
    cout_dump();
}

void foo(int j) {
    cout_dump();
}


int main() {

    Data d1 = Data(Id());
    std::cout << d1.m_iid << '\n';

    // copy-initialization via implicit conversion
#ifdef __MAKE_EXPLICIT__
    Data d2 = static_cast<int>(Id(1));
#else
    Data d2 = Id(1);
#endif
    std::cout << d2.m_iid << '\n';

    // more copy-initialization via implicit conversion
#ifdef __MAKE_EXPLICIT__
    std::cout << f(static_cast<int>(Id(2))) << '\n';
#else
    std::cout << f(Id(2)) << '\n';
#endif

    // assignment not initialization
    d2 = Id(3);
    std::cout << d2.m_iid << '\n';

    // explicit type-cast operator
    auto id = Id(4);
#ifdef __MAKE_EXPLICIT__
    if (static_cast<int>(Id(5))) cout_dump_msg("explicit");
    if (auto x = id.operator int() / 2) cout_dump_msg(x);
#else
    if (Id(5)) cout_dump_msg("implicit");
    if (auto x = id / 2) cout_dump_msg(x);
#endif

    // std::shared_ptr has explicit ctor and copyable
    // h(new Data(6)); // CE due to ctor for class 'std::shared_ptr<Data>' is declared 'explicit'
    std::shared_ptr<Data> sp(new Data(6));
    h(sp);
    std::cout << sp->m_iid << '\n';

    // std::unique_ptr has explicit ctor and nocopyable
    std::unique_ptr<Data> p(new Data(7));
    g(std::move(p));

    // overload metrics hint
    bar(8l);
    bar('c');

#ifndef __MAKE_EXPLICIT__
    // positive hack
    // foo(Id(9)); // CE 'foo': ambiguous call to overloaded function
    foo(+Id(9));
#endif
}
