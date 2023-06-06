        .att_syntax

        // All stack alignment: 16

        .text
        .global func64
        .align 16
func64:

#define ALIGN__
#if defined(ALIGN__)
        sub $8, %rsp // after use 'call' alignment
#endif

        // Now we can use scratch registers here:
        // %rsi, %rdi, %rdx, %rcx, r8 - r11, %rax
        // Callee-saved %rbx, %rbp, %rsp, r12 - r15

        // Pass parameters into
        // %rsi, %rdi, %rdx, %rcx, %r8, %r9
        // and floating-point params xmm0 - xmm7

        // Return value %rax, %rax:%rdx
        // and floating-point %xmm0

        mov $fmt, %rdi
        mov $arg, %rsi
        mov $10, %rdx
        mov $'x', %rcx

        // For %rax we should set the number of floating-point args to make
        // a 'call' of func with variable number of arguments (like printf below)
        xor %rax, %rax

        call printf

#if defined(ALIGN__)
        add $8, %rsp
#endif
        ret

fmt: .asciz "Hello, %s, %d, %c\n"
arg: .asciz "String"
