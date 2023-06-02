    .text
    .global c1
    .align 4
c1: .int 12

    .data
    .global c2, q1, q2, q3
    .align 4
c2: .int 24
    .align 8
q1: .quad 0x03, 0xDEADBEEF
    .align 4
q2: .float
    .align 8
q3: .double 2.2

    .text
    .global s1, ps1, pq3
s1: .asciz "Hello, const!\n"
    .align 8
ps1:
    .quad s1
pq3:
    .quad q3

    .data
    .global pq1_1
    .align 8
pq1_1:
    .quad q1+8
