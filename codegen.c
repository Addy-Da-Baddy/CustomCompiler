#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "lexer.h"
#include "parser.h"

void traverse_tree(Node* node, int is_left, FILE *file) {
    if (node == NULL) return;

    if (strcmp(node->value, "exit") == 0) {
        fprintf(file, "    mov rax, 60\n");
    }

    if (node->type == INT) {
        fprintf(file, "    mov rdi, %s\n", node->value);
    }

    if (node->type == OPERATOR) {
        traverse_tree(node->left, 1, file);
        if (strcmp(node->value, "+") == 0) {
            fprintf(file, "    add rdi, %s\n", node->right->value);
        } else if (strcmp(node->value, "-") == 0) {
            fprintf(file, "    sub rdi, %s\n", node->right->value);
        } else if (strcmp(node->value, "*") == 0) {
            fprintf(file, "    mov rsi, %s\n", node->right->value);
            fprintf(file, "    imul rdi,rsi \n");
        } else if (strcmp(node->value, "/") == 0) {
            fprintf(file, "    mov rax, rdi\n");                    // Move the dividend into rax
            fprintf(file, "    mov rsi, %s\n", node->right->value); // Load the divisor into rsi
            fprintf(file, "    idiv rsi\n");                        // Perform division rdx:rax / rsi
            fprintf(file, "    mov rdi, rax\n");                    // Move the quotient back to rdi
            fprintf(file, "    mov rax, 60\n");                     // Restore rax for syscall
        }
    }

    if (strcmp(node->value, ";") == 0) {
        fprintf(file, "    syscall\n");
    }

    if (node->type != OPERATOR) {
        traverse_tree(node->left, 1, file);
        traverse_tree(node->right, 0, file);
    }
}

int generate_code(Node* root) {
    FILE *file = fopen("generated.asm", "w");
    assert(file != NULL && "FILE COULD NOT BE OPENED\n");
    
    fprintf(file, "section .text\n");
    fprintf(file, "    global _start\n\n");
    fprintf(file, "_start:\n");

    traverse_tree(root, 0, file);
    fclose(file);
    return 0;
}