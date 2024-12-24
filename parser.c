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

Node* parsePrimary (Token** currentToken){
    Token* token = *currentToken;
    if (token->type == INT|| token->type == IDENTIFIER || token->type == STRING){
        Node* newNode = createNode(token->type, token->stringValue);
        (*currentToken)++;
        return newNode;
    }
    printf("Expected Primary Expression");
    exit(1);
}

Token *parser(Token* tokens){
    Token *current_token = &tokens[0];
    Node* root = createNode(current_token->type, current_token->stringValue);
    current_token++;
    if (current_token->type!=SEPARATOR || current_token->separatorValue != '('){
        printf("Expected ( \n ");
        exit(1);
    }
    else {
        Node* open_paren_node = createNode(current_token->type, "(");
        root->left = open_paren_node;
        current_token++;
    }
    if (current_token->type == INT){
        int x = current_token->intValue;
        char* strVal = (char*)malloc(100);
        sprintf(strVal, "%d", x);
        Node* numNode = createNode(current_token->type, strVal);
        root->left->left = numNode;
        current_token++;
    } else if (current_token->type == IDENTIFIER){
        Node* idNode = createNode(current_token->type, current_token->stringValue);
        root->left->left = idNode;
        current_token++;
    }
    else {
        printf("Expected identifier or integer \n");
        exit(1);
    }
      if (current_token->type!=SEPARATOR || current_token->separatorValue != ')'){
        printf("Expected ( \n ");
        exit(1);
    }
    else {
        Node* close_paren_node = createNode(current_token->type, ")");
        root->left->left->right = close_paren_node;
        current_token++;
    }
    if (current_token->type!=SEPARATOR || current_token->separatorValue!=';'){
        printf("Expected ; \n");
        exit(1);
    }
    else {
        Node* semi_node = createNode(current_token->type, ";");
        root->right = semi_node;
        current_token++;
    }
    printAST(root);
}


