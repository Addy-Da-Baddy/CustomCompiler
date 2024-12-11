#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

typedef struct{
    string
}NodeData;

typedef struct Node{
    Token token;
    struct Node* right;
    struct Node* left;
}  Node;


Node* createNode(Token token){
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode){
        printf("MemLoc failiure. Retry. \n");
        exit(1);
    }
    newNode->token = token;
    newNode->right = NULL;
    newNode->left = NULL;
}

void print_tree(Node* root){
    if (root==NULL)
        return;
    print_tree(root->left);
    printf("%d \n: ", root->value);
    print_tree(root->right);
}
Token *parser(Token* tokens){
    Token *current_token = &tokens[0];
    Node root ;
    Node left;
    Node right;
    root.value = 10;
    left.value = 20;
    right.value = 30;
    root.left = &left;
    root.right = &right;
    left.right = left.left = right.right = right.left = NULL;
    print_tree(&root);
    return current_token; 
}


