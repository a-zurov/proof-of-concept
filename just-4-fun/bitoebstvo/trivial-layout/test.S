    .text
    .align 16
    .global func
    // p - rdi
    // i - esi
    // j - edx
func:
    // p->m1[i]
    // p + i*4
    mov (%rdi, %rsi, 4), %eax

    // p->m2[j]
    // p + 64 + j*4
    mov 64(%rdi, %rdx, 4), %ecx

    add %ecx, %eax

    mov %eax, %ecx
    shr $31, %ecx // 1 for negative, 0 for positive
    add %ecx, %eax
    sar %eax

    ret


    .text
    .align 16
    .global func_bad
    // p - rdi
    // i - esi
    // j - edx
func_bad:
    // p->m1[i]
    // p + i*4
    mov (%rdi, %rsi, 4), %eax

    // p->m2[j]
    // p + 64 + j*4
    mov 64(%rdi, %rdx, 4), %ecx

    add %ecx, %eax
    sar %eax

    ret


    .align 16
    .global func_shr
func_shr:
    mov %edi, %eax
    shr $31, %eax

    ret

    .align 16
    .global func_sar
func_sar:
    mov %edi, %eax
    sar $1, %eax

    ret
