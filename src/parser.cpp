#include "parser.h"

Parser::Parser(const std::vector<Token>& tokens)
    : m_tokens(std::move(tokens)) {}

std::unique_ptr<NodeProgram> Parser::parse() {
    auto program = std::make_unique<NodeProgram>();

    while (!atEnd()) {
        program->functions.push_back(parseFunction());
    }
    return program;
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

std::unique_ptr<NodeBody> Parser::parseBody() {
    auto body = std::make_unique<NodeBody>();

    /* ... stmt1; stmt2; }*/
    while (!check(TokenType::RBRACE)) {
        body->statements.push_back(parseStatement());
    }

    consume(TokenType::RBRACE);
    return body;
}

std::unique_ptr<NodeStatement> Parser::parseStatement() {
    return nullptr; // placeholder
}

std::unique_ptr<NodeExpression> Parser::parseExpression() {
    return parseAssignment(); // placeholder
}

std::unique_ptr<NodeExpression> Parser::parseAssignment() {
    auto expr = std::make_unique<NodeExpression>();
    return expr; // placeholder
}




Token Parser::peek() const {
    return m_tokens.at(m_idx);
}

Token Parser::consume(TokenType type) {
    if (check(type)) return advance();
    throw error(peek());
}




Token Parser::advance() {
    if (!atEnd()) return m_tokens.at(m_idx++);
    return m_tokens.at(m_idx);
}

bool Parser::check(TokenType type) {
    if (atEnd() || peek().type != type) return false;
    return true;
}

bool Parser::checkAdvance(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::atEnd() {
    return peek().type == TokenType::END_OF_FILE;
}



std::runtime_error Parser::error(const Token& token) {
    // include token name -> error handling
    std::string error_message;
    return std::runtime_error(error_message);
}
