// problem_10.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// https://www.hackerrank.com/challenges/meridiem-conversion/problem

// comment this macro and check includes before copying to HackerRank editor
#define INPUT_FROM_FILE

#ifdef INPUT_FROM_FILE
#include <fstream>
#include "./../bits/bits-stdc++.h"
#else
#include <bits/stdc++.h>
#endif

using namespace std;

/*
 * Complete the 'timeConversion' function below.
 * The function is expected to return a STRING.
 * The function accepts STRING s as parameter.
 */

string timeConversion(const string& s) {

    stringstream ss(s);

    int hours, minutes;
    char colon;
    char* seconds = new char[2];
    string meridiem;

    ss >> hours >> colon >> minutes >> colon;
    ss.read(seconds, 2);
    ss >> meridiem;

    if (meridiem == "AM") {
        if (hours == 12) hours = 0;
    }
    else {
        if (hours != 12) {
            hours += 12;
        }
    }

    ss.clear();
    ss.str("");
    ss << setw(2) << setfill('0') << hours << colon
       << setw(2) << setfill('0') << minutes << colon;
    ss.write(seconds, 2);

    return ss.str();
}

int main()
{
#ifdef INPUT_FROM_FILE
    ifstream input_stream("test.txt"); // "12:45:54PM"
    if (!input_stream.is_open()) {
        std::cout << "File 'test.txt' is not found : test stopped..\n";
        return 0;
    }
#define fout std::cout
#else
#define input_stream std::cin
    ofstream fout(getenv("OUTPUT_PATH"));
#endif

    string s;
    getline(input_stream, s);

    string result = timeConversion(s);

    fout << result << "\n";

#ifdef INPUT_FROM_FILE
    input_stream.close();
#else
    fout.close();
#endif

    return 0;
}
