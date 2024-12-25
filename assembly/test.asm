section .data
    msg db "Hello, World!", 0xA ; Message to print and newline (0xA is for newline)
    len equ $ - msg              ; Calculate the length of the message

section .text
    global _start                ; Entry point for the program

_start:
    ; Step 1: Write "Hello, World!" to stdout
    mov rax, 1                   ; syscall number for 'write'
    mov rdi, 1                  ; file descriptor (1 = stdout)
    mov rsi, msg                 ; address of the message
    mov rdx, len                 ; length of the message
    syscall                      ; make the system call to print the message

    ; Step 2: Exit the program
    mov rax, 60                  ; syscall number for 'exit'
    xor rdi, rdi                 ; exit status 0
    syscall                      ; make the system call to exit
