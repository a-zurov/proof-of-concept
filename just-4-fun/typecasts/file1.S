    .att_syntax
    .text
    .global c1, c2
    .align 4
c1: .word 0xFFFF
    .align 4
c2: .byte 0x64

    .global func1, func2, func3

    .align 16
func1:
// unsigned short -> int
    movzw c1, %eax
    ret

   .align 16
func2:
// unsigned char -> int
    movzb c2, %eax
    ret

   .align 16
func3:
// signed short -> int
    movsx c1, %eax
    ret
