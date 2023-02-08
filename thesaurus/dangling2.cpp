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
};

int main() {

    int& x = Id{}.access();
    std::cout << x;
}
