#include <stdio.h>

extern const int c1;
extern int c2;
extern long long q1[];

int main() {
    printf("%d %d\n", c1, ++c2);
    printf("%llx %lld\n", q1[0], q1[1]);
}
