// dangling.cpp : This file contains the 'main' function. Program execution begins and ends there.
// g++ -fsanitize=address dangling.cpp
// ------------------------------------------

#include <iostream>

// -----------------------------------
// non-aggregates

struct S {
    char a;
    explicit S(const char& b) : a(b) {}
};

struct S1 {
    char* ptr_a;
    explicit S1(char& b) : ptr_a(&b) {}
};

struct S2 {
    char& ref_a;
    explicit S2(char& b) : ref_a(b) {}
};

// -----------------------------------
// aggregates

struct S3 {
    const char& ref_a;
};

struct S4 {
    char&& rref_a;
};

char foo() {
    return 42;
}


int main() {

    char b = 'b';

    S1 s(b);
    S2 ss(b);
    std::cout << *(s.ptr_a) << ' ' << ss.ref_a << '\n';

    const S3 * pcS3; // west-const-declared pointer to constant S3
    S2 const * pcS2; // east-const-declared pointer to constant S2

    {
        char c = 'c';
        pcS2 = new S2(c);

        S3 * const cpS3 = new S3{'a'};      // constant pointer to S3; UB: dangling reference (!)
        S2 const * const cpcS2 = new S2{b}; // constant pointer to constant S2
        pcS3 = cpS3;

        S1 const& r1 = S1(c);
        const S2& r2 = *cpcS2;

        S4 x1{'w'};     // NB: Ok in current scope only (!) due to the agregate initialization
        S4 x2{foo()};   // NB: Ok in current scope only (!)

        // No CE since C++20 (!)
        // S4 y1( 'w' );   // UB: dangling reference (!)
        // S4 y2( foo() ); // UB: dangling reference (!)
    }

    std::cout << (*pcS2).ref_a << ' ' << (*pcS3).ref_a << '\n'; // UB: dangling reference (!)

    return 0;
}
