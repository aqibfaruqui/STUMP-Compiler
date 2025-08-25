#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <optional>
#include <vector>

enum class TokenType {
    // Keywords
    FUNCTION, WHILE, IF, ELSE, RETURN,
    
    // Types
    INT, BOOL,
    
    // Literals
    INT_LIT, TRUE, FALSE, IDENTIFIER,
    
    // Operators
    PLUS, MINUS, MULTIPLY, DIVIDE, 
    ASSIGN, EQUALS,
    LESS, GREATER, LESS_EQUAL, GREATER_EQUAL,
    AND, OR, NOT,

    // Bit operations
    BIT_AND, BIT_OR, BIT_XOR, BIT_NOT,
    
    // Symbols
    LBRACKET, RBRACKET,
    LBRACE, RBRACE,
    LSQUARE, RSQUARE,
    SEMI, COMMA, DOT, ARROW,

    // STUMP Keywords
    EFFECTS,
    
    // Special
    END_OF_FILE, INVALID
};

struct Token {
   TokenType type;
   std::optional<std::string> value;
};

class Lexer {
public:
    /* Constructor */
    explicit Lexer(std::string& src);
    
    /* Converting input file to vector of tokens */
    std::vector<Token> tokenise();

private:
    /* Looking ahead n characters 
        [[nodiscard]]? used for const member functions (member var not changed)
        so return value must be of value 
        - more useful for non const functions that also return something */
    std::optional<char> peek(int ahead = 0) const;

    /* Consuming next character */
    char consume();

    const std::string m_src;
    size_t m_idx = 0;
};

#endif