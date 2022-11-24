// problem_04.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// https://www.hackerrank.com/challenges/classes-objects/problem

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
using namespace std;

// comment this macro and check includes before copying to HackerRank editor
#define INPUT_FROM_FILE

#ifdef INPUT_FROM_FILE
#include <fstream>
std::ifstream input_stream("test.txt");
#else
#define input_stream std::cin
#endif //INPUT_FROM_FILE

// Write your Student class here
class Student {
    int m_total_score = 0;

public:
    void input() {

        char next;
        while (input_stream.get(next)) {
            if (next == '\n') {
                break;
            }
        }

        for (int score, j = 0; j < 5; ++j) {
            input_stream >> score;
            m_total_score += score;
        }
    }

    int calculateTotalScore() {
        return m_total_score;
    }
};


int main()
{
#ifdef INPUT_FROM_FILE
    if (!input_stream.is_open()) {
        std::cout << "File 'test.txt' is not found : test stopped..\n";
        return 0;
    }
#endif //INPUT_FROM_FILE

    int n; // number of students
    input_stream >> n;

    Student* s = new Student[n]; // an array of n students

    for (int i = 0; i < n; i++) {
        s[i].input();
    }

    // calculate kristen's score
    int kristen_score = s[0].calculateTotalScore();

    // determine how many students scored higher than kristen
    int count = 0;
    for (int i = 1; i < n; i++) {
        int total = s[i].calculateTotalScore();
        if (total > kristen_score) {
            count++;
        }
    }

    // print result
    cout << count;

    return 0;

#ifdef INPUT_FROM_FILE
    input_stream.close();
#endif
}
