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
        printf("MemLoc failure. Retry. \n");
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

void printAST(Node* root) {
    if (root == NULL)
        return;
    char* typeS = tokenString(root->type);
    printf("Node: Type = %s, Value = %s\n", typeS, root->value ? root->value : "NULL");
    printAST(root->left);
    printAST(root->right);
}

Node* parse_expression(Token** current_token) {
    Node* exprNode = NULL;
    
    // Handle first operand
    if ((*current_token)->type == INT) {
        char* strVal = (char*)malloc(20);
        sprintf(strVal, "%d", (*current_token)->intValue);
        exprNode = createNode((*current_token)->type, strVal);
        free(strVal);
        (*current_token)++;
    } 
    else if ((*current_token)->type == IDENTIFIER) {
        exprNode = createNode((*current_token)->type, (*current_token)->stringValue);
        (*current_token)++;
    }
    else {
        printf("Expected integer or identifier\n");
        exit(1);
    }
    
    // Handle operators and subsequent operands
    while ((*current_token)->type == OPERATOR) {
        Node* opNode = createNode((*current_token)->type, (*current_token)->operatorValue);
        opNode->left = exprNode;
        (*current_token)++;
        
        if ((*current_token)->type == INT) {
            char* strVal = (char*)malloc(20);
            sprintf(strVal, "%d", (*current_token)->intValue);
            Node* rightNode = createNode((*current_token)->type, strVal);
            free(strVal);
            opNode->right = rightNode;
        }
        else if ((*current_token)->type == IDENTIFIER) {
            Node* rightNode = createNode((*current_token)->type, (*current_token)->stringValue);
            opNode->right = rightNode;
        }
        else {
            printf("Expected integer or identifier after operator\n");
            exit(1);
        }
        (*current_token)++;
        exprNode = opNode;
    }
    
    return exprNode;
}

Node* handle_exit_syscall(Token** current_token) {
    Node* current_node = createNode((*current_token)->type, (*current_token)->stringValue);
    (*current_token)++;
    
    if ((*current_token)->type != SEPARATOR || (*current_token)->separatorValue != '(') {
        printf("Expected (\n");
        exit(1);
    }
    Node* open_paren_node = createNode((*current_token)->type, "(");
    current_node->left = open_paren_node;
    (*current_token)++;
    
    // Parse expression inside parentheses
    Node* exprNode = parse_expression(current_token);
    open_paren_node->left = exprNode;
    
    if ((*current_token)->type != SEPARATOR || (*current_token)->separatorValue != ')') {
        printf("Expected )\n");
        exit(1);
    }
    Node* close_paren_node = createNode((*current_token)->type, ")");
    open_paren_node->right = close_paren_node;
    (*current_token)++;
    
    if ((*current_token)->type != SEPARATOR || (*current_token)->separatorValue != ';') {
        printf("Expected ;\n");
        exit(1);
    }
    Node* semi_node = createNode((*current_token)->type, ";");
    current_node->right = semi_node;
    (*current_token)++;
    
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
                
                if (root->left == NULL) {
                    root->left = current_node;
                } else if (last_node != NULL) {
                    last_node->right = current_node;
                }
                last_node = current_node;
                break;

            case OPERATOR:
            case IDENTIFIER:
            case INT:
            case SEPARATOR:
            case STRING:
            case COMP:
                printf("Unexpected token type: %s at start of statement\n", 
                tokenString(current_token->type));
                exit(1);
                break;

            default:
                printf("Unknown token type: %s\n", tokenString(current_token->type));
                exit(1);
        }
    }
    
    printAST(root);
    return root;
}