// dangling.cpp : This file contains the 'main' function. Program execution begins and ends there.
// g++ -fsanitize=address dangling.cpp
// ------------------------------------------

#include <iostream>

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

struct S3 {
    const char& ref_a;
};

int main() {

    char b = 'b';

    S1 s(b);
    S2 ss(b);
    std::cout << *(s.ptr_a) << ' ' << ss.ref_a << '\n';

    const S3 * pcS3; // west-end-const-declared pointer to constant S3
    S2 const * pcS2; // east-end-const-declared pointer to constant S2

    {
        char c = 'c';
        pcS2 = new S2(c);

        S3 * const cpS3 = new S3{'a'};      // constant pointer to S3
        S2 const * const cpcS2 = new S2{b}; // constant pointer to constant S2
        pcS3 = cpS3;

        S1 const& r1 = S1(c);
        const S2& r2 = *cpcS2;
    }

    std::cout << (*pcS2).ref_a << ' ' << (*pcS3).ref_a << '\n';

    return 0;
}
