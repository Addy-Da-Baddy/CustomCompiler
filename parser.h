#ifndef PARSER_H_
#define PARSER_H_



typedef struct Node{
    TokenType type;
    char* value;
    struct Node* right;
    struct Node* left;
}  Node;

Node *parser(Token* tokens);

Node* createNode(TokenType type, char* value);

void printAST(Node* root);

#endif