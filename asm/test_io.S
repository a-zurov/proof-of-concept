// gcc -m32 test.S

#include "simple_io.S"

        .att_syntax noprefix

        .global main
        .align 16
main:
        call    readi32 // eax, edx, ecx - may be overriten!
        mov     eax, ebx
        call    readi32
        add     ebx, eax
        call    writei32
        call    nl
        xor     eax, eax
        ret
