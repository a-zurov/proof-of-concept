// problem_00 : main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//---------------------------------------------------------------------------------------------------------

#include "macro.h"

#include "./../bits/bits-stdc++.h"

using namespace std;

string ltrim(const string&);
string rtrim(const string&);

/*
 * Complete the 'fizzBuzz' function below.
 *
 * The function accepts INTEGER n as parameter.
 */


void printFizzBuzz(size_t n) {

    if (n % 15 == 0) {
        cout << "FizzBuzz" << '\n';
        return;
    }
    cout << (n % 3 == 0 ? "Fizz" : (n % 5 == 0 ? "Buzz" : to_string(n))) << '\n';
}

void fizzBuzz(int n) {

    for (size_t j = 1; j < n + 1; ++j) {
        printFizzBuzz(j);
    }
}

int main()
{
    string n_temp;

    getline(cin, n_temp);

    int n = stoi(ltrim(rtrim(n_temp)));

    fizzBuzz(n);

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
