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

        .global func6
        .align 16
func6:
        /*
        a = 100500      4(%esp)
        ret address     <- esp
        */

        // Prologue { Standard Prologue {

        push %ebp
        /*
        a = 100500      8(%esp)
        ret address     4(%esp)
        previous ebp    <- esp
        */

        mov %esp, %ebp
        /*
        a = 100500      8(%esp), 8(%ebp)
        ret address     4(%esp), 4(%ebp)
        previous ebp    <- esp , <- ebp
        */
        // } Standard Prologue

        push %ebx
        push %esi
        push %edi
        /*
        a = 100500      20(%esp),   8(%ebp)
        ret address     16(%esp),   4(%ebp)
        previous ebp    12(%esp),   <- ebp
        saved ebx                  -4(%ebp)
        saved esi       4(%esp),   -8(%ebp)
        saved edi       <- esp,   -12(%ebp)
        */

        sub $4, %esp // int b; local variable
        /*
        a = 100500      24(%esp),   8(%ebp)
        ret address     20(%esp),   4(%ebp)
        previous ebp    12(%esp),   <- ebp
        saved ebx                  -4(%ebp)
        saved esi       8(%esp),   -8(%ebp)
        saved edi       4(%esp),  -12(%ebp)
        int b           <- esp,   -16(%ebp)
        */

        sub $8, %esp // stack alignment
        /*
        a = 100500      32(%esp),   8(%ebp)
        ret address     28(%esp),   4(%ebp)
        previous ebp    24(%esp),   <- ebp
        saved ebx                  -4(%ebp)
        saved esi       16(%esp),  -8(%ebp)
        saved edi       12(%esp), -12(%ebp)
        int b            8(%esp), -16(%ebp)
        padding 8       <- esp
        */
        // } Prologue

        // scanf("%d", &b);
        sub $8, %esp
        lea -16(%ebp), %eax
        push %eax
        push $fmt
        call scanf
        add $16, %esp

        mov 8(%ebp), %eax
        add -16(%ebp), %eax

        // Epilogue {
        // revert all

        add $12, %esp

        /*
        saved ebx
        saved esi
        saved edi
        */
        pop %edi
        pop %esi
        pop %ebx

        // Standard Epilogue {

        pop %ebp
        ret

        // } Standard Epilogue } Epilogue

fmt:    .asciz "%d"
