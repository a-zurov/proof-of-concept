// 100_questions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#if 0
// Q3. Which of the following operator can be overloaded through friend function ->  =  ()  *

struct S {
    int x_;
};

struct CS_Ptr
{
    S* pS_;

    CS_Ptr(S* p = nullptr) : pS_(p) {}

    friend S* operator*(const CS_Ptr& sp);
};

S* operator* (const CS_Ptr& sp) {
    return sp.pS_;
};

int main() {

    CS_Ptr sp{ new S{5} };
    std::cout << (*sp)->x_;
    delete *sp;
}
// A1. Operators that cannot be overloaded are  .*  ::  ?:
// Operators that cannot be overloaded when declaring that function as friend function are  =  ()  []  ->
#endif

//=======================================================================================================

#if 0
// Q4. What is the smallest size a variable of the type child_t may occupy in memory
// 1 bit, 7 bits, 1 byte, 12 bytes

typedef struct {
    unsigned int  age : 4;
    unsigned char gender : 1;
    unsigned int  size : 2;
} child_t;

int main() {
    std::cout << sizeof(child_t); // 12
}

// A4. 7 bits
// https://en.cppreference.com/w/cpp/language/bit_field
#endif

//=======================================================================================================

#if 1
// Q6. Which of the following is not a consequence of declaring the member variable count of my_class as static
/*
* The variable cannot be modified by any part of the code in the same application or thread. However, other threads may modify it.

The variable exists even when no objects of the class have been defined so it can be modified at any point in the source code.

The variable is allocated only once, regardless of how many objects are instantiated because it is bound to the class itself, not its instances.

All objects that try to access their count member variable actually refer to the only class - bound static count variable.
*/

class my_class {
public:
    static int count;
};

int my_class::count = 7;

int main() {

    std::cout << my_class::count << ' ';
    my_class::count = 10;
    std::cout << my_class::count;
}

// A6. The variable cannot be modified by any part of the code in the same application or thread. However, other threads may modify it.
#endif
