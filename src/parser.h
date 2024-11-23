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
    explicit Parser(const std::vector<Token>& tokens);

    /* Parsing input tokens */
    std::unique_ptr<NodeProgram> parse();

private:
    /* Parsing different abstracted constructs */
    std::unique_ptr<NodeFunction> parseFunction();
    std::unique_ptr<NodeBody> parseBody();
    std::unique_ptr<NodeStatement> parseStatement();
    std::unique_ptr<NodeAssignment> parseAssignment();
    std::unique_ptr<NodeVarDecl> parseVarDecl(Token type, bool global);
    std::unique_ptr<NodeArithmetic> parseArithmetic();
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
    Token variable;
    std::unique_ptr<NodeArithmetic> expr;
    
    NodeAssignment(Token v, std::unique_ptr<NodeArithmetic> e)
        : variable(std::move(v)), expr(std::move(e)) {}
};

struct NodeVarDecl : NodeStatement {
    Token variable;
    std::unique_ptr<NodeArithmetic> expr;
    bool global;

    NodeVarDecl(Token t, std::unique_ptr<NodeArithmetic> e, bool g = false)
        : variable(std::move(t)), expr(std::move(e)), global(g) {}
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