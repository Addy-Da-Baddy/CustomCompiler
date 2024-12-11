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


/* Node* createNode(TokenType type){
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode){
        printf("MemLoc failiure. Retry. \n");
        exit(1);
    }
    char* value = NULL;
    newNode->type = type;
    if (value){
        newNode->value = value;
    }
    else {
        newNode->value = NULL;
    }
    newNode->right = NULL;
    newNode->left = NULL;
}*/

void printAST(Node* root){
    if (root==NULL)
        return;
    print_tree(root->left);
    printf("Node: Type = %d, Value = %s\n", root->type, root->value ? root->value : "NULL");
    print_tree(root->right);
}
Token *parser(Token* tokens){
    Token *current_token = &tokens[0];
    Node root ;
    Node left;
    Node right;
    root.type = STRING;
    root.value = "Hello";
    left.type = KEYWORD;
    left.value = "print";
    right.type = INT;
    right.value = "5";
    root.left = &left;
    root.right = &right;
    left.right = left.left = right.right = right.left = NULL;
    print_tree(&root);
    return current_token; 
}


