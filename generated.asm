section .text
    global _start

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 1024
    mov rax, 60
    mov rdi, 4
    push rdi
    mov rdi, 2
    mov rsi, rdi
    pop rdi
    add rdi, rsi
    push rdi
    mov rdi, 1
    mov rsi, rdi
    pop rdi
    sub rdi, rsi
    syscall
    mov rsp, rbp
    pop rbp
