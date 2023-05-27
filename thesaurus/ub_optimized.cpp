// ub_optimized.cpp : This file contains the 'main' function.
//-----------------------------------------------------------
// NB! Try: clang -O2 ub_optimized.cpp

#include <stdlib.h>
#include <stdio.h>

typedef int (*func) ();

static func Do;

static int DoStrange() {
    return printf("I'm here!\n");
}

void NeverCalled() {
    Do = DoStrange;
}

int main() {
    return Do();
}
