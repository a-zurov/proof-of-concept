#include <stdio.h>

extern const int c1;
extern int c2;

int main() {
    printf("%d %d\n", c1, ++c2);
}
