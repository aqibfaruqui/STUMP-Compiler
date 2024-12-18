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
struct NodeAssignment;
struct NodeVarDecl;
struct NodeArithmetic;
struct NodeReturn;

class Parser {
public:
    /* Constructor */
    explicit Parser(std::vector<Token>& tokens);

    /* Parsing input tokens from lexer */
    std::unique_ptr<NodeProgram> parse();

private:
    /* Parsing different abstracted constructs */
    std::unique_ptr<NodeFunction> parseFunction();
    std::unique_ptr<NodeBody> parseBody();
    std::unique_ptr<NodeStatement> parseStatement();
    std::unique_ptr<NodeAssignment> parseAssignment();
    std::unique_ptr<NodeVarDecl> parseVarDecl(bool global);
    std::unique_ptr<NodeArithmetic> parseArithmetic();
    std::unique_ptr<NodeReturn> parseReturn();

    /* Arithmetic Helper methods */
    bool isOperator(TokenType type);
    char getOperator(TokenType op);
    int Parser::getPrecedence(char op);
    
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



struct NodeProgram {
    std::vector<std::unique_ptr<NodeFunction>> functions;
    std::vector<std::unique_ptr<NodeVarDecl>> globals;
};

struct NodeFunction {
    std::string name;
    std::vector<std::string> parameters;
    std::unique_ptr<NodeBody> body;

    NodeFunction(std::string n, std::vector<std::string> p, std::unique_ptr<NodeBody> b)
        : name(std::move(n)), parameters(std::move(p)), body(std::move(b)) {}
};

struct NodeBody {
    std::vector<std::unique_ptr<NodeStatement>> statements;
};

struct NodeStatement {
    virtual ~NodeStatement() = default;
};

struct NodeAssignment : NodeStatement {
    std::string name;
    std::unique_ptr<NodeArithmetic> expr;
    
    NodeAssignment(std::string n, std::unique_ptr<NodeArithmetic> e)
        : name(n), expr(std::move(e)) {}
};

struct NodeVarDecl : NodeStatement {
    std::string name;
    std::unique_ptr<NodeArithmetic> expr;
    bool global;

    NodeVarDecl(std::string n, std::unique_ptr<NodeArithmetic> e, bool g = false)
        : name(n), expr(std::move(e)), global(g) {}
};

struct NodeArithmetic : NodeStatement {
    Token result;

    NodeArithmetic(Token r)
        : result(std::move(r)) {}
};

struct NodeReturn : NodeStatement {
    Token return_value;

    NodeReturn(Token t)
        : return_value(std::move(t)) {}
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