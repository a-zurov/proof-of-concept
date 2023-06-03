// gcc file1.S file2.c -no-pie

#include <stdio.h>

extern const unsigned short c1;
extern const unsigned char c2;

#ifdef __cplusplus
    extern "C" {
#endif

int func1();
int func2();

#ifdef __cplusplus
    }
#endif


int main() {

    printf("%d %hu\n", func1(), c1);
    printf("%d %c\n", func2(), c2);
}
