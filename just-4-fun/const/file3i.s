    .intel_syntax noprefix
    .text
    .global func1
    .align 16
func1:
    ret

    .global func2
    .align 16
func2:
    mov eax, 42
    ret

    .global func3
    .align 16
func3:
    mov rax, q1
    ret

    .global func4
    .align 16
func4:
    mov eax, [q1]
    ret
