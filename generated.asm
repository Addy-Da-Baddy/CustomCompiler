section .text
    global _start

_start:
    mov rax, 60
    mov rdi, 7
    add rdi, 3
    sub rdi, 2
    add rdi, 3
    syscall
