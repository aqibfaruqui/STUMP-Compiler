#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <optional>

enum class TokenType {
    // Keywords
    FUNCTION, WHILE, IF, ELSE, RETURN,
    
    // Types
    INT, BOOL,
    
    // Literals
    INT_LIT, TRUE, FALSE, IDENTIFIER,
    
    // Operators
    PLUS, MINUS, MULTIPLY, DIVIDE, ASSIGN, EQUALS,
    
    // Symbols
    LBRACKET, RBRACKET, LBRACE, RBRACE, SEMI, COMMA,
    
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
    [[nodiscard]] std::optional<char> peek(int ahead = 0) const;

    /* Consuming next character */
    char consume();

    const std::string m_src;
    size_t m_idx = 0;
};

#endif