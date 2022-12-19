// problem_13.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// https://www.hackerrank.com/challenges/between-two-sets/problem

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
 * Complete the 'getTotalX' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY a
 *  2. INTEGER_ARRAY b
 */

bool CanBeDivided(int j, const vector<int>& a) {
    for (const auto& p : a) {
        if (j % p != 0) {
            return false;
        }
    }
    return true;
}

bool IsDivider(int j, const vector<int>& b) {
    for (const auto& q : b) {
        if (q % j != 0) {
            return false;
        }
    }
    return true;
}

int getTotalX(vector<int>& a, vector<int>& b) {

    sort(begin(a), end(a));
    sort(begin(b), end(b));

    vector<int> v;
    for (int j = *(a.end() - 1); j <= *b.begin(); ++j) {
        if (CanBeDivided(j, a) && IsDivider(j, b)) {
            v.push_back(j);
        }
    }
    return v.size();
}

int main()
{
#ifdef INPUT_FROM_FILE
    ifstream input_stream("test.txt"); // 3
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

    int n = stoi(first_multiple_input[0]);

    int m = stoi(first_multiple_input[1]);

    string arr_temp_temp;
    getline(input_stream, arr_temp_temp);

    vector<string> arr_temp = split(rtrim(arr_temp_temp));

    vector<int> arr(n);

    for (int i = 0; i < n; i++) {
        int arr_item = stoi(arr_temp[i]);

        arr[i] = arr_item;
    }

    string brr_temp_temp;
    getline(input_stream, brr_temp_temp);

    vector<string> brr_temp = split(rtrim(brr_temp_temp));

    vector<int> brr(m);

    for (int i = 0; i < m; i++) {
        int brr_item = stoi(brr_temp[i]);

        brr[i] = brr_item;
    }

    int total = getTotalX(arr, brr);

    fout << total << "\n";

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
