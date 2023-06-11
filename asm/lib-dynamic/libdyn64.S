// gcc -shared libdyn.S -olibdyn.so

    .att_syntax

    .text
    .global func
    .type func, @function
func:
    mov A@GOTPCREL(%rip), %rax
    mov (%rax), %eax

    mov B@GOTPCREL(%rip), %rdx
    mov (%rdx), %edx

    add %edx, %eax

    mov C@GOTPCREL(%rip), %rdx
    mov %eax, (%rdx)
    
    ret
