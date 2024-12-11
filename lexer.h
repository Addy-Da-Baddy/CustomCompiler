#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdbool.h>

// Enumeration for token types
typedef enum {
    BEGINNING,
    INT,
    KEYWORD,
    SEPARATOR,
    OPERATOR,
    IDENTIFIER,
    STRING,
    COMP,
    END_OF_TOKENS,
} TokenType;

// Structure for a token
typedef struct {
    TokenType type;
    union {
        int intValue;          // For integer literals
        char* stringValue;     // For keywords, identifiers, and strings
        char separatorValue;   // For separators like ';', '(', ')'
        char operatorValue[3]; // For operators, can hold up to two characters + null terminator
    };
} Token;

// Function declarations
Token* lexer(FILE* file);

#endif // LEXER_H
