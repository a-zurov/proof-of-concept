// structured_bindings.cpp : This file contains the 'main' function. Program execution begins and ends there.
// g++ -std=c++17 -c structured_bindings.cpp

#include <iostream>
#include <map>
#include <tuple>

std::tuple <std::string, int> CreatePerson() {
    return { "Cherno", 24 };
}

int main()
{
    std::pair<int, std::string> p1 = { 5, "abc" };

    // since C++17
    auto [a, b] = p1; // copy
    std::cout << a << ' ' << b << '\n';

    std::pair<int, std::string> p2 = { 7, "xyz" };

    std::map<int, std::string> m;
    m.insert(p1);
    m.insert(p2);

    for (const auto& [key, value] : m) {
        std::cout << key << ' ' << value << '\n';
    }

    std::tuple <int, double, std::string> t{ 3, 2.5, "abcdef" };
    auto& [c, d, e] = t;
    std::cout << c << ' ' << d << ' ' << e << '\n';

    auto x = std::get<double>(t);
    auto y = std::get<2>(t);
    std::cout << x << ' ' << y << '\n';

    auto [name, age] = CreatePerson();
    std::cout << name << ' ' << age << '\n';
}
