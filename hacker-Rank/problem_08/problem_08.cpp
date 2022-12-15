// problem_08.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// https://www.hackerrank.com/challenges/box-it/problem

// comment this macro and check includes before copying to HackerRank editor
#define INPUT_FROM_FILE

#ifdef INPUT_FROM_FILE
#include <fstream>
#include "./../bits/bits-stdc++.h"
#else
#include <bits/stdc++.h>
#endif

using namespace std;

// Implement the class Box
// l,b,h are integers representing the dimensions of the box
// The class should have the following functions :

// Constructors:
// Box();
// Box(int,int,int);
// Box(Box);

// int getLength(); // Return box's length
// int getBreadth (); // Return box's breadth
// int getHeight ();  //Return box's height
// long long CalculateVolume(); // Return the volume of the box

// Overload operator < as specified
// bool operator<(Box& b)

// Overload operator << as specified
// ostream& operator<<(ostream& out, Box& B)

class Box {
    int m_l, m_b, m_h;
public:
    Box() : m_l(0), m_b(0), m_h(0) {}
    Box(int l, int b, int h) : m_l(l), m_b(b), m_h(h) {}
    Box(const Box& box) : m_l(box.m_l), m_b(box.m_b), m_h(box.m_h) {}

    int getLength() { return m_l; }
    int getBreadth() { return m_b; }
    int getHeight() { return m_h; }
    long long CalculateVolume() { return ((long long)m_l) * m_b * m_h; }

    bool operator < (const Box& rhs) {
        return m_l < rhs.m_l ? true :
            (m_l == rhs.m_l && m_b < rhs.m_b ? true :
                (m_l == rhs.m_l && m_b == rhs.m_b && m_h < rhs.m_h ? true : false)
            );
    }

    friend ostream& operator << (ostream& os, const Box& rhs);
};

ostream& operator << (ostream& os, const Box& rhs) {
    return os << rhs.m_l << ' ' << rhs.m_b << ' ' << rhs.m_h;
}


void check2()
{
#ifdef INPUT_FROM_FILE
    ifstream input_stream("test.txt");
    if (!input_stream.is_open()) {
        std::cout << "File 'test.txt' is not found : test stopped..\n";
        return;
    }
/*
1039 3749 8473
33004122803
Greater
Lesser
4839 283 273
373856301
*/
#else
#define input_stream std::cin
#endif //INPUT_FROM_FILE

    int n;
    input_stream >> n;
    Box temp;
    for (int i = 0; i < n; i++)
    {
        int type;
        input_stream >> type;
        if (type == 1)
        {
            cout << temp << endl;
        }
        if (type == 2)
        {
            int l, b, h;
            input_stream >> l >> b >> h;
            Box NewBox(l, b, h);
            temp = NewBox;
            cout << temp << endl;
        }
        if (type == 3)
        {
            int l, b, h;
            input_stream >> l >> b >> h;
            Box NewBox(l, b, h);
            if (NewBox < temp)
            {
                cout << "Lesser\n";
            }
            else
            {
                cout << "Greater\n";
            }
        }
        if (type == 4)
        {
            cout << temp.CalculateVolume() << endl;
        }
        if (type == 5)
        {
            Box NewBox(temp);
            cout << NewBox << endl;
        }
    }
#ifdef INPUT_FROM_FILE
    input_stream.close();
#endif
}

int main()
{
    check2();
}
