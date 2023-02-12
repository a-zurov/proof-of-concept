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
#define cout_dump_msg(x) std::cout << __MAKE_DUMP__ << ' ' << x << '\n'

struct Boolean
{
    bool m_bval;

    Boolean(bool t_value) : m_bval(t_value) {}

    bool operator&&(const Boolean& right) const
    {
        cout_dump();
        return this->m_bval && right.m_bval;
    }

    bool operator||(const Boolean& right) const
    {
        cout_dump();
        return this->m_bval || right.m_bval;
    }

    operator bool() const
    {
        cout_dump();
        return m_bval;
    }
};

bool getbool(bool b) {
    cout_dump_msg(std::boolalpha << b);
    return b;
}

Boolean getBoolean(bool b) {
    cout_dump_msg(std::boolalpha << b);
    return Boolean(b);
}

struct S {
    bool m_bval{false};
    std::string m_str;

    S operator,(S s) const
    {
        cout_dump_msg(m_str);
        S tmp;
        tmp.m_bval = s.m_bval;
        tmp.m_str = s.m_str;
        return tmp;
    }

    Boolean operator&&(const S& rhs) const
    {
        cout_dump_msg(m_str << " && " << rhs.m_str);
        return m_bval && rhs.m_bval;
    }

    operator Boolean() const
    {
        cout_dump_msg(m_str);
        return m_bval;
    }
};

int main() {

    cout_dump_msg("test bool operator(&&) expecting 1 getbool(bool) call:");
    if (getbool(false) && getbool(true)) cout_dump();

    cout_dump_msg("test Boolean operator(&&) expecting 2 getBoolean(bool) calls:");
    if (getBoolean(false) && getBoolean(true)) cout_dump();

    cout_dump_msg("test operator(,)(S):");
    S s1{ false, "abc" }, s2{ true, "xyz" }, s3;
    s3 = (s1, s2);
    std::cout << std::boolalpha << s3.m_bval << ' ' << s3.m_str << '\n';

    if (s2 && s1) cout_dump();
}