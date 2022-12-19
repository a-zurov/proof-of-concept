// problem_12.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// https://www.hackerrank.com/challenges/kangaroo/problem

// comment this macro and check includes before copying to HackerRank editor
#define INPUT_FROM_FILE

#ifdef INPUT_FROM_FILE
#include <fstream>
#include "./../bits/bits-stdc++.h"
#else
#include <bits/stdc++.h>
#endif

using namespace std;

string ltrim(const string&);
string rtrim(const string&);
vector<string> split(const string&);

/*
 * Complete the 'kangaroo' function below.
 *
 * The function is expected to return a STRING.
 * The function accepts following parameters:
 *  1. INTEGER x1
 *  2. INTEGER v1
 *  3. INTEGER x2
 *  4. INTEGER v2
 */

string f(int x1, int v1, int x2, int v2) {

    if (v1 > v2) {
        do {
            x1 += v1;
            x2 += v2;
        } while (x1 < x2);
        if (x1 == x2) return "YES";
    }
    return "NO";
}

string kangaroo(int x1, int v1, int x2, int v2) {
    return x1 < x2 ? f(x1, v1, x2, v2) : f(x2, v2, x1, v1);
}

int main()
{
#ifdef INPUT_FROM_FILE
    ifstream input_stream("test.txt"); // YES
    if (!input_stream.is_open()) {
        std::cout << "File 'test.txt' is not found : test stopped..\n";
        return 0;
    }
#define fout std::cout
#else
#define input_stream std::cin
    ofstream fout(getenv("OUTPUT_PATH"));
#endif

    string first_multiple_input_temp;
    getline(input_stream, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int x1 = stoi(first_multiple_input[0]);

    int v1 = stoi(first_multiple_input[1]);

    int x2 = stoi(first_multiple_input[2]);

    int v2 = stoi(first_multiple_input[3]);

    string result = kangaroo(x1, v1, x2, v2);

    fout << result << "\n";

#ifdef INPUT_FROM_FILE
    input_stream.close();
#else
    fout.close();
#endif

    return 0;
}

string ltrim(const string& str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string& str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string& str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}

