section .text
    global _start

_start:
    mov rdi, 5
    mov rdi, 2
    mov rdi, 3
    mov rax, 60
    mov rdi, 5
    add rdi, 2
    add rdi, 3
    syscall
