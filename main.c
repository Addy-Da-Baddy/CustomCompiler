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