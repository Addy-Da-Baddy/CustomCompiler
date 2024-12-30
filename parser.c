#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

typedef struct Node {
    TokenType type;
    char* value;
    struct Node* right;
    struct Node* left;
} Node;

char* tokenString(TokenType token) {
    switch (token) {
        case BEGINNING: return "BEGINNING";
        case INT: return "INT";
        case KEYWORD: return "KEYWORD";
        case SEPARATOR: return "SEPARATOR";
        case OPERATOR: return "OPERATOR";
        case IDENTIFIER: return "IDENTIFIER";
        case STRING: return "STRING";
        case COMP: return "COMP";
        case END_OF_TOKENS: return "END_OF_TOKENS";
        default: return "UNKNOWN";
    }
}

Node* createNode(TokenType type, char* value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("MemLoc failiure. Retry. \n");
        exit(1);
    }
    newNode->type = type;
    if (value) {
        newNode->value = strdup(value);
    } else {
        newNode->value = NULL;
    }
    newNode->right = NULL;
    newNode->left = NULL;
    return newNode;
}

void printAST(Node* root, int depth) {
    if (root == NULL)
        return;

    // Indentation for visualizing depth
    for (int i = 0; i < depth; ++i) {
        printf("  "); // Indentation
    }

    // Convert type to string
    char* typeS = tokenString(root->type);
    printf("Node: Type = %s, Value = %s\n", typeS, root->value ? root->value : "NULL");

    // Recursively print children
    printAST(root->left, depth + 1);
    printAST(root->right, depth + 1);
}


Node* handle_exit_syscall(Token** current_token) {
    // Create node for 'exit'
    Node* current_node = createNode((*current_token)->type, (*current_token)->stringValue);
    (*current_token)++;  // Move to next token

    // Check if it's followed by an opening parenthesis
    if ((*current_token)->type != SEPARATOR || (*current_token)->separatorValue != '(') {
        printf("Expected (\n");
        exit(1);
    }
    Node* open_paren_node = createNode((*current_token)->type, "(");
    current_node->left = open_paren_node;
    (*current_token)++;  // Move past '('

    // Handle the expression inside the parentheses
    Node* exprNode = NULL;
    if ((*current_token)->type == IDENTIFIER || (*current_token)->type == INT) {
        // If it's an identifier or an integer, process it
        char* strVal = (char*)malloc(20);
        if ((*current_token)->type == IDENTIFIER) {
            strVal = strdup((*current_token)->stringValue);  // Store the identifier
        } else {
            sprintf(strVal, "%d", (*current_token)->intValue);  // Store the integer
        }
        exprNode = createNode((*current_token)->type, strVal);
        free(strVal);
        (*current_token)++;  // Move past the expression
    } else {
        printf("Expected an expression inside parentheses\n");
        exit(1);
    }

    // Link the expression to the open parenthesis node
    open_paren_node->left = exprNode;

    // Check for closing parenthesis ')'
    if ((*current_token)->type != SEPARATOR || (*current_token)->separatorValue != ')') {
        printf("Expected )\n");
        exit(1);
    }
    Node* close_paren_node = createNode((*current_token)->type, ")");
    open_paren_node->right = close_paren_node;
    (*current_token)++;  // Move past ')'

    // Ensure the statement ends with a semicolon
    if ((*current_token)->type != SEPARATOR || (*current_token)->separatorValue != ';') {
        printf("Expected ;\n");
        exit(1);
    }
    Node* semi_node = createNode((*current_token)->type, ";");
    current_node->right = semi_node;
    (*current_token)++;  // Move past ';'

    return current_node;
}

Node* parse_variables(Token** current_token) {
    Node* var_node = createNode((*current_token)->type, (*current_token)->stringValue);
    (*current_token)++;
    
    if ((*current_token)->type != IDENTIFIER) {
        printf("Expected Identifier\n");
        exit(1);
    }
    Node* idNode = createNode((*current_token)->type, (*current_token)->stringValue);
    var_node->left = idNode;
    (*current_token)++;

    if ((*current_token)->type != OPERATOR || strcmp((*current_token)->operatorValue, "=") != 0) {
        printf("Expected assignment \n");
        exit(1);
    }
    Node* equalNode = createNode((*current_token)->type, (*current_token)->operatorValue);
    idNode->right = equalNode;
    (*current_token)++;

    if ((*current_token)->type != INT) {
        printf("Expected integer\n");
        exit(1);
    }
    char* strVal = (char*)malloc(20);
    sprintf(strVal, "%d", (*current_token)->intValue);
    Node* valueNode = createNode((*current_token)->type, strVal);
    free(strVal);
    equalNode->left = valueNode;
    (*current_token)++;
    
    if ((*current_token)->type != SEPARATOR || (*current_token)->separatorValue != ';') {
        printf("Expected ;\n");
        exit(1);
    }
    Node* semiNode = createNode((*current_token)->type, ";");
    var_node->right = semiNode;
    (*current_token)++;
    
    return var_node;
}

Node* parser(Token* tokens) {
    Token* current_token = tokens;
    Node* root = createNode(BEGINNING, "START OF PROGRAM");
    Node* current_node = NULL;
    Node* last_node = NULL;
    
    while (current_token->type != END_OF_TOKENS) {
        switch (current_token->type) {
            case KEYWORD:
                if (strcmp(current_token->stringValue, "exit") == 0) {
                    current_node = handle_exit_syscall(&current_token);
                } 
                else if (strcmp(current_token->stringValue, "int") == 0) {
                    current_node = parse_variables(&current_token);
                }
                else {
                    printf("Unknown keyword: %s\n", current_token->stringValue);
                    exit(1);
                }
                
                // Link the nodes together
                if (root->left == NULL) {
                    root->left = current_node;
                } else if (last_node != NULL) {
                    last_node->right = current_node;
                }
                last_node = current_node;
                break;

            case OPERATOR:
                // TODO: Handle operators outside of statements
                current_token++;
                break;

            case IDENTIFIER:
                // TODO: Handle identifiers outside of statements
                current_token++;
                break;

            case INT:
                // TODO: Handle integers outside of statements
                current_token++;
                break;

            case SEPARATOR:
                // TODO: Handle separators outside of statements
                current_token++;
                break;

            case STRING:
                // TODO: Handle strings
                current_token++;
                break;

            case COMP:
                // TODO: Handle comparison operators
                current_token++;
                break;

            default:
                printf("Unexpected token type: %s\n", tokenString(current_token->type));
                exit(1);
        }
    }
    
    printAST(root,0);
    return root;
}