#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

// List of keywords for comparison
static const char* keywords[] = {
    "exit", "if", "while", "return", "int", "for", "else", "void", "break", "continue"
};
static const int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

// List of separators (punctuation characters)
static const char* separators[] = {
    ";", "(", ")", "{", "}", "[", "]", ",", "."
};
static const int num_separators = sizeof(separators) / sizeof(separators[0]);

// List of operators
static const char* operators[] = {
    "+", "-", "*", "/", "%", "=", "==", ">", "<", ">=", "<=", "&&", "||", "!", "++", "--"
};
static const int num_operators = sizeof(operators) / sizeof(operators[0]);

// Function to check if a string is a keyword
static bool isKeyword(char* keyword) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(keyword, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Function to check if a character is a separator
static bool isSeparator(char current) {
    for (int i = 0; i < num_separators; i++) {
        if (current == separators[i][0]) {
            return true;
        }
    }
    return false;
}

// Function to check if a string is an operator
static bool isOperator(const char* current) {
    for (int i = 0; i < num_operators; i++) {
        if (strcmp(current, operators[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Function to generate a keyword or identifier token
static Token generateKeywordOrIdentifier(char* current, FILE* file) {
    Token token;
    token.type = IDENTIFIER; // Default type is IDENTIFIER

    // Collect characters to form a string
    int index = 0;
    char* string = (char*)malloc(100 * sizeof(char));
    while (*current != EOF && (isalnum(*current) || *current == '_')) {
        string[index++] = *current;
        *current = fgetc(file);
    }
    string[index] = '\0'; // Null-terminate the string

    // Check if the string is a keyword
    if (isKeyword(string)) {
        token.type = KEYWORD;
        token.stringValue = string;
        return token;
    }

    // Otherwise, it's an identifier
    token.type = IDENTIFIER;
    token.stringValue = string;
    return token;
}

// Function to generate a string token (enclosed in quotes)
static Token generateStringToken(char* current, FILE* file) {
    Token token;
    token.type = STRING;

    // Collect characters inside the string
    int index = 0;
    char* string = (char*)malloc(100 * sizeof(char));
    *current = fgetc(file); // Skip the opening quote
    while (*current != EOF && *current != '"') {
        string[index++] = *current;
        *current = fgetc(file);
    }
    string[index] = '\0'; // Null-terminate the string

    *current = fgetc(file); // Move past the closing quote

    token.stringValue = string;
    return token;
}

// Function to generate separator or operator tokens
static Token generateSepOrOp(char current, FILE* file) {
    Token token;
    if (isSeparator(current)) {
        token.type = SEPARATOR;
        token.separatorValue = current;
    } else {
        token.type = OPERATOR;
        token.operatorValue[0] = current;
        token.operatorValue[1] = '\0'; // Single-character operator
    }
    return token;
}

// Function to generate multi-character operator tokens
static Token generateMultiCharOperator(char current, FILE* file) {
    Token token;
    token.type = OPERATOR;

    // Read the next character to check for multi-character operators
    char next = fgetc(file);
    if (next != EOF) {
        // Check for valid two-character operators
        if (isOperator((char[]){current, next, '\0'})) {
            token.operatorValue[0] = current;
            token.operatorValue[1] = next;
            token.operatorValue[2] = '\0'; // Null-terminate the string
        } else {
            // Single-character operator
            token.operatorValue[0] = current;
            token.operatorValue[1] = '\0'; // Null-terminate the string
            ungetc(next, file); // Push the character back to the input stream
        }
    } else {
        // Single-character operator if EOF is reached
        token.operatorValue[0] = current;
        token.operatorValue[1] = '\0'; // Null-terminate the string
    }
    return token;
}

// Lexer function to process the file#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

// Define the maximum number of tokens allowed
#define MAX_TOKENS 1000

// Updated lexer function to return a list of tokens
Token* lexer(FILE* file) {
    static Token tokens[MAX_TOKENS]; // Fixed-size token array
    int token_count = 0;

    if (file == NULL) {
        printf("Error: Could not open file.\n");
        return NULL;
    }

    char current = fgetc(file);
    while (current != EOF) {
        // Skip whitespace
        if (isspace(current)) {
            current = fgetc(file);
            continue;
        }

        // Ensure we don't exceed the token capacity
        if (token_count >= MAX_TOKENS) {
            printf("Error: Token buffer overflow. Further tokens will be ignored.\n");
            break;
        }

        // Handle separators
        if (isSeparator(current)) {
            tokens[token_count++] = generateSepOrOp(current, file);
            current = fgetc(file);
        }
        // Handle operators, including multi-character operators
        else if (strchr("+-*/%!=><&|", current)) {
            tokens[token_count++] = generateMultiCharOperator(current, file);
            current = fgetc(file);
        }
        // Handle integer literals
        else if (isdigit(current)) {
            Token token;
            token.type = INT;
            token.intValue = current - '0';
            while (isdigit((current = fgetc(file)))) {
                token.intValue = token.intValue * 10 + (current - '0');
            }
            tokens[token_count++] = token;
        }
        // Handle string literals
        else if (current == '"') {
            tokens[token_count++] = generateStringToken(&current, file);
        }
        // Handle keywords and identifiers
        else if (isalpha(current) || current == '_') {
            tokens[token_count++] = generateKeywordOrIdentifier(&current, file);
        }
        // Handle unrecognized characters
        else {
            current = fgetc(file);
        }
    }

    // Null-terminate the token list for easy processing (optional, requires END_OF_TOKENS handling)
    if (token_count < MAX_TOKENS) {
        tokens[token_count].type = END_OF_TOKENS;
    }

    return tokens;
}
