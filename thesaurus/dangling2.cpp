// dangling2.cpp : This file contains the 'main' function. Program execution begins and ends there.
// g++ -fsanitize=address dangling2.cpp
// ------------------------------------------

#include <iostream>

struct Id {
    int m_id;

    Id(int j = 0) : m_id(j) {}

    int& access() {
        return m_id;
    }

    int& better_access() & { // ref-annotation
        return m_id;
    }
};

int main() {

    int& x = Id{}.access(); // dangling reference
    std::cout << x << '\n'; // UB (!)

    // int y = Id{}.better_access(); // CE: a non-const reference may only be bound to an lvalue
    // const int& z = Id{}.better_access(); // CE: cannot convert 'this' pointer from 'Id' to 'Id &'

    {
        Id id;
        int& w = id.better_access(); // Ok in this scope
        std::cout << w;
    }
}
