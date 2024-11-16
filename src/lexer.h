#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <optional>

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

struct Token {
   TokenType type;
   std::optional<std::string> value;
};

#endif