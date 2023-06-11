// gcc -m32 -shared libdyn.S -olibdyn.so

    .att_syntax

    .text
    .global func
    .type func, @function
func:
    push %ebx
    call getip
    mov $_GLOBAL_OFFSET_TABLE_, %ebx

    mov A@GOT(%ebx), %eax
    mov (%eax), %eax

    mov B@GOT(%ebx), %edx
    mov (%edx), %edx

    add %edx, %eax

    mov C@GOT(%ebx), %edx
    mov %eax, (%edx)

    pop %ebx
    ret

getip:
    mov (%esp), %ebx
    ret
