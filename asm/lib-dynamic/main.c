// gcc -m32 main.c libdyn.S -omain -no-pie
// gcc -m32 main.c -L . -ldyn -omain -Wl,-rpath,`pwd`

#include <stdio.h>

int A, B, C;

void func(void);

int main()
{
    scanf("%d", &A);
    scanf("%d", &B);
    func();
    printf("%d\n", C);
}
