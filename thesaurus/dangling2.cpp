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

    int& better_access() & // ref-annotation
    {
        return m_id;
    }

    static int m_sid;

    static int& static_access() // ref-qualifiers are not allowed here
    {
        return m_sid;
    }
};

int Id::m_sid = 1;

int main() {

    int& x = Id{}.access(); // dangling reference
    std::cout << x << '\n'; // UB (!)

    // int& y = Id{}.better_access(); // CE passing ‘Id’ as ‘this’ argument discards qualifiers
    // const int& z = Id{}.better_access(); // CE

    {
        Id id;
        int& w = id.better_access(); // Ok in this scope
        std::cout << w << '\n';
    }

    std::cout << Id::static_access();
}
