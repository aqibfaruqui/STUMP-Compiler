#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>

#include "lexer.h"

enum class TokenType {
    // Keywords
    FUNCTION,
    WHILE,
    IF,
    ELSE,
    RETURN,
    
    // Types
    INT_LIT,
    BOOL,
    
    // Literals
    NUMBER,
    TRUE,
    FALSE,
    IDENTIFIER,
    
    // Operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    EQUALS,
    
    // Symbols
    LBRACKET,
    RBRACKET,
    LBRACE,
    RBRACE,
    SEMI,
    COMMA,
    
    // Special
    END_OF_FILE,
    INVALID
};

int main() {
    return 0;
}