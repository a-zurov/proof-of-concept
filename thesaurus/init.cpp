// init.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------
//

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

//-------------------------------------------------
// The most vexing parse is a counterintuitive form
// of syntactic ambiguity resolution in the C++

struct list_t {};
struct myclass_t {
    int x = 42;
    myclass_t(list_t, list_t) {}
};

//-------------------------------------------------
// std::initializer_list supremacy ----------------
struct S {
    int k{42};
    S(int j, bool x) : k(j) { std::cout << "ctor with bool" << '\n'; }
    S(int j, double x) : k(j) { std::cout << "ctor with double" << '\n'; }
    S(int j, std::string x) : k(j) { std::cout << "ctor with std::string" << '\n'; }
    S(std::initializer_list<int> l) { std::cout << "ctor with std::initializer_list" << '\n'; }
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

    // the most vexing parse in direct initialization --------------
    myclass_t m1( (list_t()), (list_t() ) ); // (A) the most vexing parse without parentheses
    myclass_t m2{ list_t(), list_t() };

    std::cout << m1.x << '\n'; // CE without additional parentheses in (A)
    std::cout << m2.x << '\n';

    // the idea of uniform initialization --------------------------
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
    //S s3{ 10, .5 };    // CE invalid narrowing conversion from double to int
    //S s4{ 10, "abc" }; // CE conversion from 'const char [4]' to 'bool' requires a narrowing conversion
    using namespace std::string_literals;
    S s5{ 10, "abc"s };

    // aggregate initialization by the initializer list ----------
    // since C++17
    Derived d{ { {5}, .2f }, 'a' };
};
