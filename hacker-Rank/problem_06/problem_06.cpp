// problem_00 : main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//---------------------------------------------------------------------------------------------------------
// https://www.hackerrank.com/challenges/attending-workshops

using namespace std;

// comment this macro and check includes before copying to HackerRank editor
#define INPUT_FROM_FILE

#ifdef INPUT_FROM_FILE
#include <fstream>
#include "./../bits/bits-stdc++.h"
#endif

//Define the structs Workshops and Available_Workshops.
//Implement the functions initialize and CalculateMaxWorkshops

struct  Workshop {
    int m_start_time;
    int m_duration;
    int m_end_time;
};

struct Available_Workshops {
    int m_size;
    Workshop* m_parrWorkshops;
};

Available_Workshops* initialize(int start_time[], int duration[], int n) {

    Workshop* parrWorkshops = new Workshop[n];
    for (int j = 0; j < n; ++j) {
        *(parrWorkshops + j) = { start_time[j], duration[j], start_time[j] + duration[j] };
    }
    return new Available_Workshops{ n, parrWorkshops };
}

int CalculateMaxWorkshops(Available_Workshops* ptr) {

    sort(ptr->m_parrWorkshops, ptr->m_parrWorkshops + ptr->m_size,
        [](Workshop rhs, Workshop lhs) {
            return rhs.m_end_time < lhs.m_end_time;
        });

    int curr_end_time = (ptr->m_parrWorkshops)->m_end_time;
    int counter = 1;
    for (int j = 1; j < ptr->m_size; ++j) {
        int next_start_time = (ptr->m_parrWorkshops + j)->m_start_time;
        if (curr_end_time <= next_start_time) {
            ++counter;
            curr_end_time = (ptr->m_parrWorkshops + j)->m_end_time;
        }
    }
    return counter;
}

int main(int argc, char* argv[]) {

#ifdef INPUT_FROM_FILE
    ifstream input_stream("test.txt"); // 27
    if (!input_stream.is_open()) {
        std::cout << "File 'test.txt' is not found : test stopped..\n";
        return 0;
    }
#else
#define input_stream std::cin
#endif //INPUT_FROM_FILE

    int n; // number of workshops
    input_stream >> n;
    // create arrays of unknown size n
    int* start_time = new int[n];
    int* duration = new int[n];

    for (int i = 0; i < n; i++) {
        input_stream >> start_time[i];
    }
    for (int i = 0; i < n; i++) {
        input_stream >> duration[i];
    }

    Available_Workshops* ptr;
    ptr = initialize(start_time, duration, n);
    cout << CalculateMaxWorkshops(ptr) << endl;

#ifdef INPUT_FROM_FILE
    input_stream.close();
#endif

    return 0;
}
