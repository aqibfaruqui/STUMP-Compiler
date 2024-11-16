#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "lexer.h"
#include "parser.h"

/*explicit*/ Parser::Parser(const std::vector<Token>& tokens)
    : m_tokens(std::move(tokens)) {}

/*[[nodiscard]]*/ std::optional<Token> Parser::peek(int ahead) const {
    if (m_idx + ahead >= m_tokens.size()) {
        return {};
    } 
    return m_tokens.at(m_idx + ahead);    
}

Token Parser::consume() {
    return m_tokens.at(m_idx++);
}
