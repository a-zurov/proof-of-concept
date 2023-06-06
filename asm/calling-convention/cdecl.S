        .att_syntax

        .text
        .global func1
        .align 16
func1:
        mov $0x12345678, %eax
        mov $0x99AABBCC, %edx

        ret

        .global func2
        .align 16
func2:
        push %ebp
        push %ebx
        push %esi
        push %edi

        // Now we can use here
        // scratch registers: %eax, %edx, %ecx
        // + callee-saved: %ebp, %ebx, %esi, %edi

        pop %edi
        pop %esi
        pop %ebx
        pop %ebp

        ret

        .global func3
        .align 16
func3:

#define ALIGN__
#if defined(ALIGN__)
        sub $12, %esp // after use 'call' alignment
        // try gdb: b func3, info registers, n, x $esp
#endif

        // printf("Hello, %d, %c\n", 10, 'x');

//#define DIRECT__
#if defined(DIRECT__)

        pushl   $'x'
        pushl   $10
        pushl   $str

#else // NOT DIRECT__

#if defined(ALIGN__)
        sub     $16, %esp
#else // NOT ALIGN__
        sub     $12, %esp
#endif // NOT ALIGN__

        movl    $str, (%esp)
        movl    $10, 4(%esp)    // [%esp + 4]
        movl    $'x', 8(%esp)   // [%esp + 8]

#endif // NOT DIRECT__

        call printf

#if defined(DIRECT__)

        popl %eax
        popl %eax
        popl %eax

#else // NOT DIRECT__

#if defined(ALIGN__)
        add $16, %esp
#else // NOT ALIGN__
        add $12, %esp
#endif // NOT ALIGN__

#endif // NOT DIRECT__

#if defined(ALIGN__)
        add $12, %esp
#endif // ALIGN__

        ret

str:    .asciz "Hello, %d, %c\n"


        .global func4
        .align 16
func4:
        // b = 4747
        // a = 1234
        // ret address <- esp

        mov 4(%esp), %eax
        add 8(%esp), %eax

        ret

        .global func5
        .align 16
func5:
        // b = 4747
        // a = 1234
        // ret address <- esp

        mov 4(%esp), %eax
        add 8(%esp), %eax

        ret
