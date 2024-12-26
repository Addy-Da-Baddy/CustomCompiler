#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "codegen.h"


int main() {
    FILE* file = fopen("test.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open the file.\n");
        return 1;
    }

    printf("File contents:\n");
    Token* tokens = lexer(file);
    for (int i = 0; i!=-1 ; i++) {
        printf("Token %d: ", i);
        switch (tokens[i].type) {
            case BEGINNING:
                printf("BEGINNING\n");
                break;
            case INT:
                printf("INT: %d\n", tokens[i].intValue);
                break;
            case KEYWORD:
                printf("KEYWORD: %s\n", tokens[i].stringValue);
                break;
            case SEPARATOR:
                printf("SEPARATOR: %c\n", tokens[i].separatorValue);
                break;
            case OPERATOR:
                printf("OPERATOR: %s\n", tokens[i].operatorValue);
                break;
            case IDENTIFIER:
                printf("IDENTIFIER: %s\n", tokens[i].stringValue);
                break;
            case STRING:
                printf("STRING: %s\n", tokens[i].stringValue);
                break;
            case COMP:
                printf("COMP: %s\n", tokens[i].stringValue);
                break;
            case END_OF_TOKENS:
                printf("END_OF_TOKENS\n");
                break;    
        }
        if (tokens[i].type == END_OF_TOKENS) break;   
    }
    Node* test = parser(tokens);
    generate_code(test);
    FILE *assembly_file = fopen("generated.asm", "r");
    if (!assembly_file){
        printf("ERROR");
        exit(1);
    } 
    fclose(file);
    return 0;
}