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

    const uint32_t mod = 1 << 31;

    uint64_t N, S, P, Q;
    input_stream >> N >> S >> P >> Q;

    uint64_t current = S % mod;
    uint64_t next;

    map< uint64_t, size_t> mapIntCnt;
    mapIntCnt[current] = 0;

    for (uint64_t j = 1; j < N; ++j) {

        next = (current * P + Q) % mod;

        if (mapIntCnt.count(next)) ++mapIntCnt[next];
        else mapIntCnt.insert({ next, 0 });
        current = next;
    }

#ifdef INPUT_FROM_FILE
    for (auto m : mapIntCnt) {
        cout << m.first << " : " << m.second << '\n';
    }
    input_stream.close();
#endif

    cout << mapIntCnt.size() << '\n';

    return 0;
}
