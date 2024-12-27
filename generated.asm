section .text
    global _start

_start:
    mov rax, 60
    mov rdi, 4
    mov rax, rdi
    mov rsi, 2
    idiv rsi
    mov rdi, rax
    mov rax, 60
    add rdi, 6
    sub rdi, 1
    syscall
