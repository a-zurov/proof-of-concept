// gcc -m32 main.c cdecl.S

#include <stdio.h>

#ifdef __cplusplus
    extern "C" {
#endif

long long func1(void);
void func3(void);
int func4(int a, int b);

struct Args {
    int a;
    int b;
};
int func5(struct Args x);

int func6(int a);

#ifdef __cplusplus
    }
#endif


int main() {

    long long res1 = func1();
    printf("%llx\n", res1);

    func3();

    int res2 = func4(1234, 4747);
    printf("%d\n", res2); // 5981

    struct Args args = {1234, 4747};
    int res3 = func5(args);
    printf("%d\n", res3); // 5981

    int res4 = func6(100500);
    printf("%d\n", res4); // 100500 + scanf("%d", &b);
}
