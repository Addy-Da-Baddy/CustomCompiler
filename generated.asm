section .text
    global _start

_start:
    push rbp
    mov rbp, rsp
    mov rdi, 7
    mov rdi, 3
    mov rdi, 7
    push rdi
    mov rdi, 3
    pop rsi
    imul rdi, rsi
    push rdi
    mov rdi, 1
    pop rsi
    sub rsi, rdi
    mov rdi, rsi
    mov rax, 60
    syscall
    mov rsp, rbp
    pop rbp
