#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <optional>
#include <vector>
#include <memory>
#include "lexer.h"


struct NodeExpression;
struct NodeStatement;
struct NodeFunction;
struct NodeBlock;
struct NodeProgram;

class Parser {
public:
    /* Constructor */
    explicit Parser(const std::vector<Token>& tokens);

    /* Parsing input tokens */
    std::unique_ptr<NodeProgram> parse();

private:
    /* Parsing different abstracted constructs */
    std::unique_ptr<NodeExpression> parseExpression();
    std::unique_ptr<NodeExpression> parseAssignment();
    std::unique_ptr<NodeStatement> parseStatement();
    std::unique_ptr<NodeBlock> parseBlock();
    std::unique_ptr<NodeFunction> parseFunction();

    /* Navigating and validating tokens */
    Token advance();
    bool check(TokenType type);
    bool checkAdvance(TokenType type);
    bool atEnd();

    /* Looking at/consuming previous/current token, should we use ahead for peek? */
    std::optional<Token> peek() const;
    Token previous() const;
    Token consume(TokenType type);

    const std::vector<Token> m_tokens;
    size_t m_idx = 0;
};

#endif