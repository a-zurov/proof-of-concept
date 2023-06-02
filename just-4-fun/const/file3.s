    .text
    .global func1
    .align 16
func1:
    ret

    .global func2
    .align 16
func2:
    movl $42, %eax
    ret

    .global func3
    .align 16
func3:
    movq $q1, %rax
    ret

    .global func4
    .align 16
func4:
    movl q1, %eax
    ret
