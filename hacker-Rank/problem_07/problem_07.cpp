// problem_07.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -------------------------------------------------------------------------------------------------
// https://www.hackerrank.com/challenges/bitset-1/problem

// comment this macro and check includes before copying to HackerRank editor
#define INPUT_FROM_FILE

#ifdef INPUT_FROM_FILE
#include <fstream>
#include "./../bits/bits-stdc++.h"
#include "macro.h"
#endif

using namespace std;

int main(int argc, char* argv[]) {

#ifdef INPUT_FROM_FILE
    ifstream input_stream("test.txt"); // 26
    if (!input_stream.is_open()) {
        std::cout << "File 'test.txt' is not found : test stopped..\n";
        return 0;
    }
    DumpDuration dd("main");
#else
#define input_stream std::cin
#endif //INPUT_FROM_FILE

    constexpr uint32_t bit_mod = (1 << 31) - 1;

    uint64_t N, S, P, Q;
    input_stream >> N >> S >> P >> Q;

    uint64_t tortoise = S & bit_mod;
    uint64_t hare = tortoise;
    uint64_t j = 1;

    do {
        tortoise = (tortoise * P + Q) & bit_mod;

        hare = (hare * P + Q) & bit_mod;
        hare = (hare * P + Q) & bit_mod;

    } while (++j < N && tortoise != hare);

    cout << j << '\n';

#ifdef INPUT_FROM_FILE
    input_stream.close();
#endif

    return 0;
}
