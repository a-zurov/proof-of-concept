// cdecl.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------
// resolve cdecl: char* (*(&some_t)[10]) (const int*&);

#include <iostream>

typedef char* (*(&ref_arr_ptr_func_t)[10]) (const int*&);

void print(const ref_arr_ptr_func_t arr) {

    const int n = 10;
    const int* pcn = &n;
    for (const auto& p : arr) {
        std::cout << *p(pcn);
    }
}

static char a = 'a';
static char b = 'b';

char* f(const int*& x) {

    if (10 == *x) {
        return &a;
    }
    return &b;
}

void g() {
    std::cout << "*";
}

int main() {

    void (*a[3]) ();
    std::fill(std::begin(a), std::end(a), g);
    for (const auto& pg : a) pg();

    const int j = 0;
    const int* pcj = &j;

    using ptr_func_t = char* (*)(const int*&);
    ptr_func_t pf = f;
    std::cout << *(*pf)(pcj) << '\n';

    char* (*arr[10]) (const int*&);
    std::fill(std::begin(arr), std::end(arr), f);
    print(arr);
}
