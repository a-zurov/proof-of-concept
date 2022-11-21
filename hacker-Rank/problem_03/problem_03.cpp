// problem_03.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
//

#include "./../bits/bits-stdc++.h"

using namespace std;

#define INPUT_FROM_FILE

int main()
{
#ifdef INPUT_FROM_FILE
    ifstream input_stream("test.txt");
    if (!input_stream.is_open()) {
        std::cout << "File 'test.txt' is not found : test stopped..\n";
        return 0;
    }
#else
#define input_stream std::cin
#endif //INPUT_FROM_FILE





#ifdef INPUT_FROM_FILE
    input_stream.close();
#endif //INPUT_FROM_FILE
}
