// problem_07.cpp : This file contains the 'main' function. Program execution begins and ends there.
// -------------------------------------------------------------------------------------------------
// https://www.hackerrank.com/challenges/bitset-1/problem

// comment this macro and check includes before copying to HackerRank editor
#define INPUT_FROM_FILE

#ifdef INPUT_FROM_FILE
#include <fstream>
#include "./../bits/bits-stdc++.h"
#endif

using namespace std;

int main(int argc, char* argv[]) {

#ifdef INPUT_FROM_FILE
    ifstream input_stream("test.txt"); // 25
    if (!input_stream.is_open()) {
        std::cout << "File 'test.txt' is not found : test stopped..\n";
        return 0;
    }
#else
#define input_stream std::cin
#endif //INPUT_FROM_FILE

    const uint32_t mod = 1 << 31;

    uint64_t N, S, P, Q;
    input_stream >> N >> S >> P >> Q;

    uint64_t current = S % mod;
    uint64_t next;

    map< uint64_t, size_t> mapRes;

    mapRes[current] = 0;

    for (uint64_t j = 1; j < N; ++j) {

        next = (current * P + Q) % mod;

        if (mapRes.count(next)) ++mapRes[next];
        else mapRes.insert({ next, 0 });

        current = next;

        //cout << j << " : "  << mapRes.size() << '\n';
    }

    size_t counter = 0;
    for (auto m : mapRes) {

        if (0 == m.second) ++counter;

        //cout << m.first << " : " << m.second << '\n';
    }

    cout << counter;

#ifdef INPUT_FROM_FILE
    input_stream.close();
#endif

    return 0;
}
