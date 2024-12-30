section .text
    global _start

_start:
    push rbp
    mov rbp, rsp
    sub rsp, 1024
    mov QWORD [rbp - 8], 3
    mov rsp, rbp
    pop rbp
