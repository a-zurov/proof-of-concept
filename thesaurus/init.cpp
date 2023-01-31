// init.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------
//

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

//-------------------------------------------------
// The most vexing parse is a counterintuitive form
// of syntactic ambiguity resolution in the C++.

struct list_t {};
struct myclass_t {
    int x = 42;
    myclass_t(list_t, list_t) {}
};

//-------------------------------------------------
// std::initializer_list supremacy ----------------
struct S {
    int k{42};
    S(int j, bool Ü) : k(j) { std::cout << "ctor1" << '\n'; }
    S(int j, double d) : k(j) { std::cout << "ctor2" << '\n'; }
    S(std::initializer_list<int> l) { std::cout << "ctor3" << '\n'; }
};

//-------------------------------------------------
// aggregate initialization (see below)

struct Base {
    int j;
};

struct Middle : public Base {
    float t;
};

struct Derived : public Middle {
    char c;
};

//-------------------------------------------------

int main() {

    // most vexing parse in direct initialization --------------
    myclass_t m1( (list_t()), (list_t() ) ); // (A) most vexing parse without parentheses
    myclass_t m2{ list_t(), list_t() };

    std::cout << m1.x << '\n'; // CE without additional parentheses in (A)
    std::cout << m2.x << '\n';

    // the idea of uniform initialization -----------------------
    std::thread th{
        []() {
            auto start = std::chrono::high_resolution_clock::now();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            auto stop = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed{ stop - start };
            std::cout << std::this_thread::get_id() << ':' << elapsed.count() << " ms" << '\n';
        }
    };
    th.join();

    // std::initializer_list supremacy ---------------------------
    S s0( 10, false );
    S s1{ 10, false };
    S s2( 10, .5 );
    //S s3{ 10, .5 }; // CE invalid narrowing conversion from double to int

    // aggregate initialization by the initializer list ----------
    // since C++17
    Derived d{ { {5}, .2f }, 'a' };
};
