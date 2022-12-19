// problem_11.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// https://www.hackerrank.com/challenges/migratory-birds/problem

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
 * Complete the 'migratoryBirds' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts INTEGER_ARRAY arr as parameter.
 */

using pair_t = pair<int, int>;

int migratoryBirds(vector<int>& arr) {

    sort(begin(arr), end(arr));

    set<pair_t> s;
    auto it = upper_bound(arr.begin(), arr.end(), arr[0]);
    s.insert({ it - arr.begin(), arr[0] });
    while (it != arr.end()) {
        auto it2 = upper_bound(it, arr.end(), *it);
        s.insert({ it2 - it, *it });
        it = it2;
    }

    auto it3 = s.rbegin();
    do {
        ++it3;
    } while (it3 != s.rend() && it3->first == s.rbegin()->first);

    return (--it3)->second;
}

int main()
{
#ifdef INPUT_FROM_FILE
    ifstream input_stream("test.txt"); // 4
    if (!input_stream.is_open()) {
        std::cout << "File 'test.txt' is not found : test stopped..\n";
        return 0;
    }
#define fout std::cout
#else
#define input_stream std::cin
    ofstream fout(getenv("OUTPUT_PATH"));
#endif

    string arr_count_temp;
    getline(input_stream, arr_count_temp);

    int arr_count = stoi(ltrim(rtrim(arr_count_temp)));

    string arr_temp_temp;
    getline(input_stream, arr_temp_temp);

    vector<string> arr_temp = split(rtrim(arr_temp_temp));

    vector<int> arr(arr_count);

    for (int i = 0; i < arr_count; i++) {
        int arr_item = stoi(arr_temp[i]);

        arr[i] = arr_item;
    }

    int result = migratoryBirds(arr);

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
