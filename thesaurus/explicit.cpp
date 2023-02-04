// explicit.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

struct NAS { // non-aggregate structure

    int _n;
    char _c;

    __EXPLICIT__ NAS() : _n(0), _c(0) {
        cout_dump();
    }
    /*__EXPLICIT__*/ NAS(int n) : _n(n), _c(0) {
        cout_dump();
    }
    __EXPLICIT__ NAS(int n, char c) : _n(n), _c(c) {
        cout_dump();
    }

    NAS(const NAS& s) : _n(s._n), _c(s._c) {
        cout_dump();
    }

    NAS& operator= (const NAS& rhs) {
        cout_dump();
        _n = rhs._n;
        _c = rhs._c;
    }

    __EXPLICIT__ operator bool() const {
        cout_dump();
        return (_n == _c);
    }
};

template <typename T>
void f(const T& s) {
    cout_dump();
}

using S = NAS;

int main() {

#ifdef __MAKE_EXPLICIT__

    S s1{};
    //S s2 = {}; // CE no appropriate default constructor available
    S s3 = S{};

    //f<S>({}); // CE void f<S>(const T&)': cannot convert argument from 'initializer list' to 'const T&'
    f<S>(S{});

    S s4{ 3 };
    S s5 = { 3 };
    S s6 = S{ 3 };

    f<S>({ 3 });
    f<S>(S{ 3 });

    S s7{ 3, 5 };
    //S s8 = {3, 5}; // CE copy-list-initialization cannot use an explicit constructor
    S s9 = S{ 3, 5 };

    //f<S>({3, 5}); // CE void f<S>(const T&)': cannot convert argument from 'initializer list' to 'const T&'
    f<S>(S{ 3, 5 });

    if (s3) cout_dump();
    if (S{ 1024 }) cout_dump();
    //if (S{ 1024 } + 5) cout_dump(); // CE there is no acceptable conversion
    //bool b1 = S{ 1024 }; // CE 'initializing': cannot convert from 'S' to 'bool'
    bool b2 = static_cast<bool>(S{ 1024 });

#else

    S s1{};
    S s2 = {};
    S s3 = S{};

    f<S>({});
    f<S>(S{});

    S s4{ 3 };
    S s5 = { 3 };
    S s6 = S{ 3 };

    f<S>({ 3 });
    f<S>(S{ 3 });

    S s7{ 3, 5 };
    S s8 = { 3, 5 };
    S s9 = S{ 3, 5 };

    f<S>({ 3, 5 });
    f<S>(S{ 3, 5 });

    if (s3) cout_dump();
    if (S{ 1024 }) cout_dump();
    if (S{ 1024 } + 5) cout_dump();
    bool b1 = S{ 1024 };
    bool b2 = static_cast<bool>(S{ 1024 });

#endif // __MAKE_EXPLICIT__
}
