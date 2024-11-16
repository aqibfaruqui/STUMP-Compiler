#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "lexer.h"


Lexer::Lexer(const std::string& src) 
    : m_src(std::move(src)), m_idx(0) {}

std::vector<Token> Lexer::tokenise() {
    std::vector<Token> tokens;
    std::string buffer;

    while (peek().has_value()) {

        /* SKIP WHITESPACE */
        if (std::isspace(peek().value())) consume();
            
        /* INT_LIT */
        else if (std::isdigit(peek().value())) 
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

        /* TEXT-BASED TOKENS */
        else if (std::isalpha(peek().value())) {
            buffer.push_back(consume());
            while (peek().has_value() && std::isalnum(peek().value())) {
                buffer.push_back(consume());
            }
            if (buffer == "return")         tokens.push_back({.type = TokenType::RETURN});
            else if (buffer == "if")        tokens.push_back({.type = TokenType::IF});
            else if (buffer == "while")     tokens.push_back({.type = TokenType::WHILE});
            else if (buffer == "else")      tokens.push_back({.type = TokenType::ELSE});
            else if (buffer == "function")  tokens.push_back({.type = TokenType::FUNCTION});
            else                            tokens.push_back({.type = TokenType::IDENTIFIER, .value = buffer});
            buffer.clear();
        }

        /* SINGLE CHARACTER TOKENS */
        else {
            switch(peek().value()) {
                case ';':
                    tokens.push_back({.type = TokenType::SEMI});
                    break;
                case '(':
                    tokens.push_back({.type = TokenType::LBRACKET});
                    break;
                case ')':
                    tokens.push_back({.type = TokenType::RBRACKET});
                    break;
                case '{':
                    tokens.push_back({.type = TokenType::LBRACE});
                    break;
                case '}':
                    tokens.push_back({.type = TokenType::RBRACE});
                    break;
                default:
                    std::cerr << "invalid character" << std::endl;
                    exit(EXIT_FAILURE);
            }
            consume();
        }
    }
    m_idx = 0;
    return tokens;
}

std::optional<char> Lexer::peek(int ahead) const {
    if (m_idx + ahead >= m_src.length()) {
        return {};
    } 
    return m_src.at(m_idx + ahead);    
}

char Lexer::consume() {
    return m_src.at(m_idx++);
}
