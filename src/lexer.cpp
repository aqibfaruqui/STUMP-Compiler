#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>

enum class TokenType {
    // Keywords
    FUNCTION,
    WHILE,
    IF,
    ELSE,
    RETURN,
    
    // Types
    INT,
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
    SEMICOLON,
    COMMA,
    
    // Special
    END_OF_FILE,
    INVALID
};

int main() {
    return 0;
}