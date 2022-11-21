// problem_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// Parse a string of comma separated integers.
//

#include <sstream>
#include <vector>
#include <iostream>

using string = std::string;

std::vector<int> parseInts(string str) {
    std::stringstream ss(str);
    std::vector<int> v;
    for (string line; std::getline(ss, line, ','); ) {
        v.push_back(stoi(line));
    }
    return v;
}

int main() {
    string str;
    std::cin >> str;
    std::vector<int> integers = parseInts(str);
    for (int i = 0; i < integers.size(); i++) {
        std::cout << integers[i] << "\n";
    }
    return 0;
}
