#include <stdio.h>

extern const int c1;
extern int c2;
extern long long q1[];
extern float q2;
extern double q3;

extern const char s1[];
extern const char* const ps1;
extern double * const pq3;
extern long long * const pq1_1;

int main() {

    printf("%d %d\n", c1, ++c2);
    printf("%llx %lld\n", q1[0], q1[1]);
    printf("%p %p\n", q1, q1+1);
    printf("%p %p\n", &q2, &q3);
    printf("%f %f\n", q2, q3);
    printf("%s", s1);
    printf("%s", ps1);
    printf("%f\n", *pq3);
    printf("%p\n", pq3);
    printf("%lld\n", *pq1_1);
    printf("%p\n", pq1_1);
}
