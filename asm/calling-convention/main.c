// gcc -m32 main.c cdecl.S

#include <stdio.h>

#ifdef __cplusplus
    extern "C" {
#endif

long long func1(void);
void func3(void);

#ifdef __cplusplus
    }
#endif


int main() {

    long long res1 = func1();
    printf("%llx\n", res1);

    func3();
}
