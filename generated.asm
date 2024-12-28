section .text
    global _start

_start:
    mov rdi, 9
    mov rax, 60
    mov rdi, 6
    syscall
