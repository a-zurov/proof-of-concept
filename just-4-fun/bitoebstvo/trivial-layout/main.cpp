// g++ main.cpp test.S
#include <iostream>

using namespace std;

struct S {
    int m1[16];
    int m2[16];
};

extern "C" int func(S* p, int i, int j);
// Q: func should return (p->m1[i] + p->m2[j])/2


extern "C" int func_bad(S* p, int i, int j);
extern "C" int func_shr(int k);
extern "C" int func_sar(int k);

int main() {

    cout << sizeof(S) << '\n';
    cout << offsetof(S, m1) << '\n';
    cout << offsetof(S, m2) << '\n';

    S s = {{1, 3, -4}, {-4, 2, -4}};

    printf("%d %d\n", func(&s, 0, 0), func_bad(&s, 0, 0));
    printf("%d %d\n", func(&s, 1, 1), func_bad(&s, 1, 1));
    printf("%d %d\n", func(&s, 2, 2), func_bad(&s, 2, 2));
}
