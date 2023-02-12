// avada_kedavra.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

struct S {
    int m_id{};
    std::string m_str;

    S operator,(S s)
    {
        cout_dump();
        S tmp;
        tmp.m_id = s.m_id;
        tmp.m_str = s.m_str;
        return tmp;
    }
};

int main() {

    S s1{ 1,"abc" }, s2{ 2,"xyz" }, s3;
    s3 = (s1, s2);
    std::cout << s3.m_id << ' ' << s3.m_str;
}