// init2.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------

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

#define cout_dump() std::cout << __MAKE_DUMP__ << '\n'
#define cout_dump_msg(x) std::cout << __MAKE_DUMP__ << ' ' << x << '\n'

#define __MAKE_EXPLICIT__ // <- switch here (!)

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

    operator Data() const;
};

struct Data {
    int m_iid;

    explicit Data(Id id) : m_iid(id.m_id) {
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

int f(const Data& d) {
    cout_dump_msg(d.m_iid);
    return d.m_iid;
}

int main() {

    Data d1 = Data(Id());
    std::cout << d1.m_iid << '\n';

    // copy-initialization via implicit conversion
    Data d2 = Id(1);
    std::cout << d2.m_iid << '\n';

    // more implicit conversion
    std::cout << f(Id(2)) << '\n';

    // assignment not initialization
    d2 = Id(3);
    std::cout << d2.m_iid << '\n';
}
