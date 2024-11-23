#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <memory>
#include <utility>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include "lexer.h"

struct NodeProgram;
struct NodeFunction;
struct NodeBody;
struct NodeStatement;
struct NodeExpression;
struct NodeVarDecl;
struct NodeArithmetic;
struct NodeReturn;

class Parser {
public:
    /* Constructor */
    explicit Parser(const std::vector<Token>& tokens);

    /* Parsing input tokens */
    std::unique_ptr<NodeProgram> parse();

private:
    /* Parsing different abstracted constructs */
    std::unique_ptr<NodeFunction> parseFunction();
    std::unique_ptr<NodeBody> parseBody();
    std::unique_ptr<NodeStatement> parseStatement();
    std::unique_ptr<NodeExpression> parseExpression();
    std::unique_ptr<NodeExpression> parseAssignment();
    std::unique_ptr<NodeVarDecl> parseVarDecl(Token type, bool global);
    int parseArithmetic();
    std::unique_ptr<NodeReturn> parseReturn();

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

struct NodeVarDecl : NodeStatement {
    TokenType type;
    std::string name;
    std::unique_ptr<NodeExpression> expr;
    bool global;

    NodeVarDecl(TokenType t, std::string n, std::unique_ptr<NodeExpression> e, bool g = false)
        : type(std::move(t)), name(std::move(n)), expr(std::move(e)), global(g) {}
};


struct NodeProgram {
    std::vector<std::unique_ptr<NodeFunction>> functions;
    std::vector<std::unique_ptr<NodeVarDecl>> globals;
};

struct NodeAssignmentExpr : NodeExpression {
    std::unique_ptr<NodeExpression> left;
    std::unique_ptr<NodeExpression> right;

    NodeAssignmentExpr(std::unique_ptr<NodeExpression> l, std::unique_ptr<NodeExpression> r)
        : left(std::move(l)), right(std::move(r)) {}
};

// struct NodeArithmetic : NodeExpression {

// };

struct NodeReturn : NodeExpression {

};

#endif

/*
 *
 *  Program -> 
 *             Global vars
 *             Functions    -> 
 *                              Function name
 *                              Parameters
 *                              Function body   ->
 *                                                  Statements ->
 *                                                                  Statement  (if, while, return, loops etc)
 *                                                                  Expression (value-producing constants)          ->
 *                                                                                                                      BIDMAS?
 *                                                                  Assignment (variable declaration/reassignment)  -> 
 *                                                                                                                      allocating memory?!   
 *
 * 
 *
 *
 *
 *
 *
 */