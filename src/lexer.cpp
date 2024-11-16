#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "lexer.h"


/*explicit*/ Lexer::Lexer(const std::string& src) 
    : m_src(std::move(src)), m_idx(0) {}

std::vector<Token> Lexer::tokenise() {
    std::vector<Token> tokens;
    std::string buffer;

    while (peek().has_value()) {
            
        /* INT_LIT */
        if (std::isdigit(peek().value())) 
        {
            buffer.push_back(consume());
            while (peek().has_value() && std::isdigit(peek().value())) {
                buffer.push_back(consume());
            }
            if (std::isspace(peek().value()) || peek().value() == ';') {
                tokens.push_back({.type = TokenType::INT_LIT, .value = buffer});
                buffer.clear();
            } else {
                std::cerr << "invalid integer" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        /* RETURN */
        else if (std::isalpha(peek().value())) {
            buffer.push_back(consume());
            while (peek().has_value() && std::isalnum(peek().value())) {
                buffer.push_back(consume());
            }
            if (buffer == "return") {
                tokens.push_back({.type = TokenType::RETURN});
                buffer.clear();
            } else {
                std::cerr << "not return" << std::endl;
                exit(EXIT_FAILURE);
            }
        } 

        /* SEMI */
        if (peek().has_value() && peek().value() == ';') {
            tokens.push_back({.type = TokenType::SEMI});
            consume();
        } 

        /* SKIP WHITESPACE */
        if (peek().has_value() && std::isspace(peek().value())) consume();
    }
    m_idx = 0;
    return tokens;
}

/*[[nodiscard]]*/ std::optional<char> Lexer::peek(int ahead) const {
    if (m_idx + ahead >= m_src.length()) {
        return {};
    } 
    return m_src.at(m_idx + ahead);    
}

char Lexer::consume() {
    return m_src.at(m_idx++);
}
