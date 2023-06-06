// gcc -no-pie main64.c cdecl64.S

#include <stdio.h>

#ifdef __cplusplus
    extern "C" {
#endif

void func64(void);

#ifdef __cplusplus
    }
#endif


int main() {
    func64();
}
