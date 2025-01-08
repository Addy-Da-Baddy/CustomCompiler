#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "lexer.h"
#include "parser.h"
#include "hashmap.h"

static HashMap* variables;

void init_generator() {
    variables = hashmap_create();
}

void cleanup_generator() {
    hashmap_free(variables);
}

// Helper function to handle expressions
void handle_expression(Node* node, FILE* file) {
    if (node->type == INT) {
        fprintf(file, "    mov rdi, %s\n", node->value); // Load integer value into rdi
    } 
    else if (node->type == IDENTIFIER) {
        int value = hashmap_get(variables, node->value);
        if (value == -1) {
            printf("Error: Undefined variable %s\n", node->value);
            exit(1);
        }
        fprintf(file, "    mov rdi, %d\n", value); // Load variable value into rdi
    }
    else if (node->type == OPERATOR) {
        // Left operand
        handle_expression(node->left, file);
        fprintf(file, "    push rdi\n"); // Save left operand on the stack

        // Right operand
        handle_expression(node->right, file);
        fprintf(file, "    pop rsi\n");  // Load left operand into rsi

        if (strcmp(node->value, "+") == 0) {
            fprintf(file, "    add rdi, rsi\n"); // rdi = rdi + rsi
        } 
        else if (strcmp(node->value, "-") == 0) {
            fprintf(file, "    sub rsi, rdi\n"); // rsi = rsi - rdi
            fprintf(file, "    mov rdi, rsi\n"); // Move result to rdi
        } 
        else if (strcmp(node->value, "*") == 0) {
            fprintf(file, "    imul rdi, rsi\n"); // rdi = rdi * rsi
        } 
        else if (strcmp(node->value, "/") == 0) {
            fprintf(file, "    mov rax, rsi\n"); // Move left operand to rax
            fprintf(file, "    cqo\n");         // Sign-extend rax into rdx:rax
            fprintf(file, "    idiv rdi\n");    // rax = rax / rdi
            fprintf(file, "    mov rdi, rax\n"); // Move result to rdi
        }
    }
}

void traverse_tree(Node* node, FILE* file) {
    if (node == NULL) return;

    // Handle variable declarations
    if (node->type == KEYWORD && strcmp(node->value, "int") == 0) {
        Node* id_node = node->left;
        Node* assign_node = id_node->right;
        
        // Calculate the right side
        handle_expression(assign_node->left, file);
        
        // Store in variable
        int value = atoi(assign_node->left->value); // Get the value from the AST
        hashmap_put(variables, id_node->value, value);
    }

    // Handle exit syscall
    else if (node->type == KEYWORD && strcmp(node->value, "exit") == 0) {
        // Handle expression inside exit()
        handle_expression(node->left->left, file);
        fprintf(file, "    mov rax, 60\n"); // Syscall number for exit
        fprintf(file, "    syscall\n");     // Invoke exit syscall
    }

    // Continue traversing if not handled above
    traverse_tree(node->left, file);
    traverse_tree(node->right, file);
}

int generate_code(Node* root) {
    FILE *file = fopen("generated.asm", "w");
    assert(file != NULL && "FILE COULD NOT BE OPENED\n");
    
    fprintf(file, "section .text\n");
    fprintf(file, "    global _start\n\n");
    fprintf(file, "_start:\n");
    fprintf(file, "    push rbp\n");
    fprintf(file, "    mov rbp, rsp\n");

    init_generator();
    traverse_tree(root, file);
    cleanup_generator();
    
    fprintf(file, "    mov rsp, rbp\n");
    fprintf(file, "    pop rbp\n");
    fclose(file);
    return 0;
}