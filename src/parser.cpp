#include "parser.h"

Parser::Parser(const std::vector<Token>& tokens)
    : m_tokens(std::move(tokens)) {}

std::unique_ptr<NodeProgram> Parser::parse() {
    auto program = std::make_unique<NodeProgram>();

    while (!atEnd()) {
        program->functions.push_back(parseFunction());
    }
    return program
}

std::unique_ptr<NodeFunction> Parser::parseFunction() {
    /* function name(... */
    consume(TokenType::FUNCTION);
    auto name = consume(TokenType::IDENTIFIER).value;
    consume(TokenType::LBRACKET);

    /* param1, param2)...*/
    std::vector<std::string> parameters;
    if (!check(TokenType::RBRACKET)) {
        do {
            parameters.push_back(consume(TokenType::IDENTIFIER).value.value());
        } while (checkAdvance(TokenType::COMMA));
    }
    consume(TokenType::RBRACKET);

    /* { (body) ... */
    consume(TokenType::LBRACE);
    auto body = parseBody(); 

    /* Function: name (parameters) {body} */
    return std::make_unique<NodeFunction>(name.value(), std::move(parameters), std::move(body));
}

std::optional<Token> Parser::peek() const {
    if (m_idx + ahead >= m_tokens.size()) {
        return {};
    } 
    return m_tokens.at(m_idx + ahead);    
}

Token Parser::consume() {
    return m_tokens.at(m_idx++);
}
