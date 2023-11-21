#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Token type enumeration
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_CONSTANT,
    TOKEN_STRING,
    TOKEN_SPECIAL_SYMBOL,
    TOKEN_OPERATOR,
    TOKEN_ERROR,
    TOKEN_END_OF_FILE
} TokenType;

// Structure to represent a token
typedef struct {
    TokenType type;
    char value[100];
} Token;

// Global variables for source code and position
char sourceCode[1000];
int currentPosition = 0;

// Keywords in your language (modify as needed)
const char* keywords[] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while","printf","scanf"};

// Special symbols in your language (modify as needed)
const char* specialSymbols[] = {"{", "}", "(", ")", ";", ",", "[", "]"};

// Function to initialize the lexer
void initializeLexer(const char* code) {
    strncpy(sourceCode, code, sizeof(sourceCode) - 1);
    sourceCode[sizeof(sourceCode) - 1] = '\0';
    currentPosition = 0;
}

// Function to get the next character from the source code
char getNextChar() {
    return sourceCode[currentPosition++];
}

// Function to check if a character is a valid identifier character
bool isValidIdentifierChar(char c) {
    return isalnum(c) || c == '_';
}

bool isKeyword(const char* str) {
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Function to get the next token
Token getNextToken() {
    Token token;
    char currentChar = getNextChar();

    while (isspace(currentChar)) {
        currentChar = getNextChar();  // Skip whitespace
    }

    if (currentChar == '\0') {
        token.type = TOKEN_END_OF_FILE;
        return token;
    }

    if (isalpha(currentChar) || currentChar == '_') {
        int i = 0;
        token.value[i++] = currentChar;
        while (isValidIdentifierChar(sourceCode[currentPosition])) {
            token.value[i++] = getNextChar();
        }
        token.value[i] = '\0';

        // Check if the identifier is a keyword
        if (isKeyword(token.value)) {
            token.type = TOKEN_KEYWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
    } 
    else if (isdigit(currentChar)) {
        int i = 0;
        token.value[i++] = currentChar;
        while (isdigit(sourceCode[currentPosition])) {
            token.value[i++] = getNextChar();
        }
        token.value[i] = '\0';
        token.type = TOKEN_CONSTANT;
    } 
    else if (currentChar == '"') {
        int i = 0;
        token.value[i++] = currentChar;
        while (sourceCode[currentPosition] != '"' && sourceCode[currentPosition] != '\0') {
            token.value[i++] = getNextChar();
        }
        if (sourceCode[currentPosition] == '"') {
            token.value[i++] = getNextChar();  // Include the closing double-quote
            token.value[i] = '\0';
            token.type = TOKEN_STRING;
        } else {
            token.type = TOKEN_ERROR;
            strcpy(token.value, "Unclosed string");
        }
    }
    else if (currentChar == ';' || currentChar == '{' || currentChar == '}' || currentChar == '(' || currentChar == ')' || currentChar == ',' || currentChar == '[' || currentChar == ']')
    {
        token.value[0] = currentChar;
        token.value[1] = '\0';
        token.type = TOKEN_SPECIAL_SYMBOL;
    }
    else {
        int i = 0;
        token.value[i++] = currentChar;
        char twoChars[3];
        twoChars[0] = currentChar;
        twoChars[1] = sourceCode[currentPosition];
        twoChars[2] = '\0';

        // Check for two-character operators or symbols
        for (int j = 0; j < sizeof(specialSymbols) / sizeof(specialSymbols[0]); j++) {
            if (strcmp(twoChars, specialSymbols[j]) == 0) {
                token.value[i++] = getNextChar();
                break;
            }
        }

        // Check for one-character operators or symbols
        for (int j = 0; j < sizeof(specialSymbols) / sizeof(specialSymbols[0]); j++) {
            if (token.value[0] == specialSymbols[j][0]) {
                break;
            }
        }

        token.value[i] = '\0';
        token.type = TOKEN_SPECIAL_SYMBOL;
    }

    return token;
}

int main() {
    char inputCode[1000];
    printf("Enter your code: ");
    fgets(inputCode, sizeof(inputCode), stdin);
    initializeLexer(inputCode);

    Token token;
    do {
        token = getNextToken();

        // Process or print the token here
        switch (token.type) {
            case TOKEN_KEYWORD:
                printf("Keyword: %s\n", token.value);
                break;
            case TOKEN_IDENTIFIER:
                printf("Identifier: %s\n", token.value);
                break;
            case TOKEN_CONSTANT:
                printf("Constant: %s\n", token.value);
                break;
            case TOKEN_STRING:
                printf("String: %s\n", token.value);
                break;
            case TOKEN_SPECIAL_SYMBOL:
                printf("Special Symbol: %s\n", token.value);
                break;
            case TOKEN_OPERATOR:
                printf("Operator: %s\n", token.value);
                break;
            case TOKEN_ERROR:
                printf("Error: %s\n", token.value);
                break;
            case TOKEN_END_OF_FILE:
                printf("End of File\n");
                break;
        }
    } while (token.type != TOKEN_END_OF_FILE);

    return 0;
}
