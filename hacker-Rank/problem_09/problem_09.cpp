// problem_09.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -------------------------------------------------------------------------------------------------
// https://www.hackerrank.com/challenges/cpp-variadics/problem

#include <iostream>

// comment this macro and check includes before copying to HackerRank editor
#define INPUT_FROM_FILE

#ifdef INPUT_FROM_FILE
#include <fstream>
#endif

using namespace std;

// Enter your code for reversed_binary_value<bool...>()
template <bool arg1>
int reversed_binary_value() {
    return arg1;
}
template <bool arg1, bool arg2, bool... args>
int reversed_binary_value() {
    return (reversed_binary_value<arg2, args...>() << 1) + arg1;
}

template <int n, bool...digits>
struct CheckValues {
    static void check(int x, int y)
    {
        CheckValues<n - 1, 0, digits...>::check(x, y);
        CheckValues<n - 1, 1, digits...>::check(x, y);
    }
};

template <bool...digits>
struct CheckValues<0, digits...> {
    static void check(int x, int y)
    {
        int z = reversed_binary_value<digits...>();
        std::cout << (z + 64 * y == x);
    }
};

int main()
{
#ifdef INPUT_FROM_FILE
    ifstream input_stream("test.txt");
    if (!input_stream.is_open()) {
        std::cout << "File 'test.txt' is not found : test stopped..\n";
        return 0;
    }
/*
0100000000000000000000000000000000000000000000000000000000000000
0000000000100000000000000000000000000000000000000000000000000000
*/
#else
#define input_stream std::cin
#endif //INPUT_FROM_FILE

    int t; input_stream >> t;

    for (int i = 0; i != t; ++i) {
        int x, y;
        input_stream >> x >> y;
        CheckValues<6>::check(x, y);
        cout << "\n";
    }

#ifdef INPUT_FROM_FILE
    input_stream.close();
#endif
}
