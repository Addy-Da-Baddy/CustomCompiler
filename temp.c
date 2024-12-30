#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "lexer.h"
#include "parser.h"
#include "hashmap.h"

void generateOperatorCode(Node* node, FILE* file);

// Global variable map for tracking stack offsets
HashMap* variables;
int next_offset = 8;  // Start after saved rbp

void traverse_tree(Node* node, int is_left, FILE *file) {
    if (node == NULL) return;

    // Handle variable declarations
    if (node->type == KEYWORD && strcmp(node->value, "int") == 0) {
        Node* id_node = node->left;  // Get identifier node
        Node* equal_node = id_node->right;  // Get equals node
        Node* value_node = equal_node->left;  // Get value node
        
        // Add variable to hashmap with next available offset
        hashmap_put(variables, id_node->value, next_offset);
        next_offset += 8;
        
        // Generate assembly for assignment
        if (value_node->type == INT) {
            int offset;
            hashmap_get(variables, id_node->value, &offset);
            fprintf(file, "    mov QWORD [rbp - %d], %s\n", 
                   offset, value_node->value);
        }
        return;
    }

    // Handle exit syscall
    if (strcmp(node->value, "exit") == 0) {
        fprintf(file, "    mov rax, 60\n");
        traverse_tree(node->left, 1, file);
        traverse_tree(node->right, 0, file);
        return;
    }

    // Handle variable references
    if (node->type == IDENTIFIER) {
        int offset;
        if (hashmap_get(variables, node->value, &offset)) {
            fprintf(file, "    mov rdi, QWORD [rbp - %d]\n", offset);
        } else {
            fprintf(stderr, "Undefined variable: %s\n", node->value);
            exit(1);
        }
        return;
    }

    // Handle integer literals
    if (node->type == INT) {
        fprintf(file, "    mov rdi, %s\n", node->value);
        return;
    }

    // Handle operators
    if (node->type == OPERATOR) {
        generateOperatorCode(node, file);
        return;
    }

    // Handle semicolon for syscall
    if (strcmp(node->value, ";") == 0) {
        fprintf(file, "    syscall\n");
    }

    traverse_tree(node->left, 1, file);
    traverse_tree(node->right, 0, file);
}

void generateOperatorCode(Node* node, FILE* file) {
    // First, evaluate left side
    traverse_tree(node->left, 1, file);
    
    // Save left result if needed
    fprintf(file, "    push rdi\n");
    
    // Evaluate right side
    traverse_tree(node->right, 0, file);
    
    // Move right result to rsi
    fprintf(file, "    mov rsi, rdi\n");
    
    // Restore left result
    fprintf(file, "    pop rdi\n");

    // Perform operation
    if (strcmp(node->value, "+") == 0) {
        fprintf(file, "    add rdi, rsi\n");
    } else if (strcmp(node->value, "-") == 0) {
        fprintf(file, "    sub rdi, rsi\n");
    } else if (strcmp(node->value, "*") == 0) {
        fprintf(file, "    imul rdi, rsi\n");
    } else if (strcmp(node->value, "/") == 0) {
        fprintf(file, "    mov rax, rdi\n");
        fprintf(file, "    xor rdx, rdx\n");
        fprintf(file, "    idiv rsi\n");
        fprintf(file, "    mov rdi, rax\n");
    } else {
        fprintf(stderr, "Unrecognized operator: %s\n", node->value);
    }
}

int generate_code(Node* root) {
    FILE *file = fopen("generated.asm", "w");
    assert(file != NULL && "FILE COULD NOT BE OPENED\n");
    
    // Initialize variable tracking
    variables = hashmap_create();
    
    // Set up the assembly file
    fprintf(file, "section .text\n");
    fprintf(file, "    global _start\n\n");
    fprintf(file, "_start:\n");
    
    // Set up stack frame
    fprintf(file, "    push rbp\n");
    fprintf(file, "    mov rbp, rsp\n");
    fprintf(file, "    sub rsp, 1024\n");  // Reserve plenty of stack space
    
    // Generate code from AST
    traverse_tree(root, 0, file);
    
    // Clean up stack frame
    fprintf(file, "    mov rsp, rbp\n");
    fprintf(file, "    pop rbp\n");
    
    fclose(file);
    hashmap_free(variables);
    return 0;
}