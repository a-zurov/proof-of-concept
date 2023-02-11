// best_for.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------

#include <iostream>
#include <vector>

int main() {

    std::vector<int> v{ 1,2,3 };

    for (auto it = v.begin(), it_end = v.end(); it != it_end; ++it) {
        std::cout << *it << '\n';
    }
}