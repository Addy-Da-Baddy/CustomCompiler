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
    printAST(root->left);
    printf("Node: Type = %d, Value = %s\n", root->type, root->value ? root->value : "NULL");
    printAST(root->right);
}
Token *parser(Token* tokens){
    Token *current_token = &tokens[0];
    Node* root = createNode(STRING, "hello world");
    Node* left = createNode(KEYWORD, "print");
    Node* right = createNode(INT, "10");
    root->left = left;
    root->right = right;
    printAST(root);
    return current_token; 
}


