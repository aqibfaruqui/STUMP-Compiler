#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include "lexer.h"


struct NodeExpression;
struct NodeStatement;
struct NodeFunction;
struct NodeBody;
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
    std::unique_ptr<NodeBody> parseBody();
    std::unique_ptr<NodeFunction> parseFunction();

    /* Looking at/consuming previous/current token, should we use ahead for peek? */
    Token peek() const;
    Token consume(TokenType type);

    /* Navigating and validating tokens */
    Token advance();
    bool check(TokenType type);
    bool checkAdvance(TokenType type);
    bool atEnd();

    /* Error handling */
    std::runtime_error error(const Token& token);

    const std::vector<Token> m_tokens;
    size_t m_idx = 0;
};




struct NodeExpression {
    virtual ~NodeExpression() = default;
};

struct NodeStatement {
    virtual ~NodeStatement() = default;
};

struct NodeBody : NodeStatement {
    std::vector<std::unique_ptr<NodeStatement>> statements;
};

struct NodeFunction : NodeStatement {
    std::string name;
    std::vector<std::string> parameters;
    std::unique_ptr<NodeBody> body;

    NodeFunction(std::string n, std::vector<std::string> p, std::unique_ptr<NodeBody> b)
        : name(std::move(n)), parameters(std::move(p)), body(std::move(b)) {}
};

struct NodeProgram {
    std::vector<std::unique_ptr<NodeFunction>> functions;
};

struct NodeAssignmentExpr : NodeExpression {
    std::unique_ptr<NodeExpression> left;
    std::unique_ptr<NodeExpression> right;

    NodeAssignmentExpr(std::unique_ptr<NodeExpression> l, std::unique_ptr<NodeExpression> r)
        : left(std::move(l)), right(std::move(r)) {}
}; 

#endif