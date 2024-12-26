#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

typedef struct Node{
    TokenType type;
    char* value;
    struct Node* right;
    struct Node* left;
}  Node;

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

Node* createNode(TokenType type, char* value){
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode){
        printf("MemLoc failiure. Retry. \n");
        exit(1);
    }
    newNode->type = type;
    if (value){
        newNode->value = value;
    }
    else {
        newNode->value = NULL;
    }
    newNode->right = NULL;
    newNode->left = NULL;
    return newNode;
}


void printAST(Node* root){
    if (root==NULL)
        return;
    char* typeS = tokenString(root->type);
    printf("Node: Type = %s, Value = %s\n", typeS, root->value ? root->value : "NULL");
    printAST(root->left);
    printAST(root->right);
}

Node* parser(Token* tokens) {
    Token* current_token = &tokens[0];
    Node* root = createNode(current_token->type, current_token->stringValue);
    current_token++;

    if (current_token->type != SEPARATOR || current_token->separatorValue != '(') {
        printf("Expected (\n");
        exit(1);
    }
    Node* open_paren_node = createNode(current_token->type, "(");
    root->left = open_paren_node;
    current_token++;

    // Parse first number
    if (current_token->type != INT) {
        printf("Expected integer\n");
        exit(1);
    }
    char* strVal = (char*)malloc(100);
    sprintf(strVal, "%d", current_token->intValue);
    Node* numNode = createNode(current_token->type, strVal);
    current_token++;

    // If next token is an operator, create operator node
    if (current_token->type == OPERATOR) {
        Node* opNode = createNode(current_token->type, current_token->operatorValue);
        opNode->left = numNode;
        current_token++;

        // Parse second number
        if (current_token->type != INT) {
            printf("Expected integer after operator\n");
            exit(1);
        }
        strVal = (char*)malloc(100);
        sprintf(strVal, "%d", current_token->intValue);
        Node* secondNum = createNode(current_token->type, strVal);
        opNode->right = secondNum;
        current_token++;
        
        // Attach operator node to open parenthesis
        open_paren_node->left = opNode;
    } else {
        // Single number case
        open_paren_node->left = numNode;
    }

    if (current_token->type != SEPARATOR || current_token->separatorValue != ')') {
        printf("Expected )\n");
        exit(1);
    }
    Node* close_paren_node = createNode(current_token->type, ")");
    root->left->right = close_paren_node;
    current_token++;

    if (current_token->type != SEPARATOR || current_token->separatorValue != ';') {
        printf("Expected ;\n");
        exit(1);
    }
    Node* semi_node = createNode(current_token->type, ";");
    root->right = semi_node;
    printAST(root);
    return root;
}

