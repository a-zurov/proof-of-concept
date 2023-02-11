// best_for.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------

#include <iostream>
#include <vector>
#include <ctime>

struct S {
    int m_id;
    std::string m_str;
};

int main() {

    int count{ 0 };

    clock_t start, fin;

    std::vector<S> v( 1000000, { 1, "abc" } );

    start = clock();

    for (auto it = v.begin(), it_end = v.end(); it != it_end; ++it) {
        if((*it).m_str == "abc") count += (*it).m_id;
    }

    fin = clock();

    std::cout << ( (double)(fin - start) / CLOCKS_PER_SEC ) << '\n';

    count = 0;

    start = clock();

    for (auto it = v.begin(); it != v.end(); ++it) {
        if ((*it).m_str == "abc") count += (*it).m_id;
    }

    fin = clock();

    std::cout << ((double)(fin - start) / CLOCKS_PER_SEC);
}