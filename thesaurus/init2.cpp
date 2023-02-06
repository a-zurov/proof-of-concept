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

#define __MAKE_EXPLICIT__ // <- switch here (!)

#ifdef __MAKE_EXPLICIT__
#define __EXPLICIT__ explicit
#else
#define __EXPLICIT__
#endif


struct Id {
    int m_id;
    Id(int j = 0) : m_id(j) {
        cout_dump();
    }
};

struct Data {
    int m_iid;
    Data(Id id) : m_iid(id.m_id) {
        cout_dump();
    }
};

int main() {

    Data d1 = Data(Id());
    std::cout << d1.m_iid;
}
