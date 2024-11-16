#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <optional>

enum class TokenType {
    // Keywords
    FUNCTION, WHILE, IF, ELSE, RETURN,
    
    // Types
    INT, BOOL, VOID,
    
    // Literals
    INT_LIT, TRUE, FALSE, IDENTIFIER,
    
    // Operators
    PLUS, MINUS, MULTIPLY, DIVIDE, 
    ASSIGN, EQUALS,
    LESS, GREATER, LESS_EQUAL, GREATER_EQUAL,
    AND, OR, NOT,

    // Bit operations
    BIT_AND, BIT_OR, BIT_XOR, BIT_NOT,
    SHIFT_LEFT, SHIFT_RIGHT,
    
    // Symbols
    LBRACKET, RBRACKET, LBRACE, RBRACE, 
    SEMI, COMMA,

    // FPGA Keywords
    SET_LED, CLEAR_LED,
    SET_CURSOR, PRINT_LCD,
    READ_KEY, WAIT_KEY,  
    DELAY,                  
    
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
    explicit Lexer(const std::string& src);
    
    /* Converting input file to vector of tokens */
    std::vector<Token> tokenise();

private:
    /* Looking ahead n characters */
    [[nodiscard]] inline std::optional<char> peek(int ahead = 0) const;

    /* Consuming next character */
    inline char consume();

    const std::string m_src;
    size_t m_idx = 0;
};

#endif