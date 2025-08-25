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
struct NodeExpression;
struct NodeInteger;
struct NodeIdentifier;
struct NodeOperator;
struct NodeFunctionCall;

class Parser {
public:
    /* Constructor */
    explicit Parser(std::vector<Token>& tokens);

    /* Parsing input tokens from lexer */
    std::unique_ptr<NodeProgram> parse();

private:
    /* Parsing different abstracted constructs */
    std::unique_ptr<NodeFunction> parseFunction();
    std::vector<std::string> parseParameters();
    void parseEffectList();
    std::unique_ptr<NodeBody> parseBody();
    std::unique_ptr<NodeStatement> parseStatement();
    std::unique_ptr<NodeAssignment> parseAssignment();
    std::unique_ptr<NodeVarDecl> parseVarDecl(bool global);
    std::unique_ptr<NodeArithmetic> parseArithmetic();
    std::unique_ptr<NodeReturn> parseReturn();

    /* Arithmetic Helper methods */
    bool isOperator(TokenType type);
    char getOperator(TokenType op);
    int getPrecedence(char op);

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

/* Layers of abstract syntax tree
 * ---> Program -> Function -> Body -> Statement -> Expression
 */
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
    
    NodeBody(std::vector<std::unique_ptr<NodeStatement>> stmts)
        : statements(std::move(stmts)) {}
};

//---> Statement ∈ {Assignment, VarDecl, Return}
struct NodeStatement {
    virtual ~NodeStatement() = default;
};

struct NodeAssignment : NodeStatement {
    std::string name;
    std::unique_ptr<NodeArithmetic> rpn;
    
    NodeAssignment(std::string n, std::unique_ptr<NodeArithmetic> r)
        : name(n), rpn(std::move(r)) {}
};

struct NodeVarDecl : NodeStatement {
    std::string name;
    std::unique_ptr<NodeArithmetic> rpn;
    bool global;

    NodeVarDecl(std::string n, std::unique_ptr<NodeArithmetic> r, bool g = false)
        : name(n), rpn(std::move(r)), global(g) {}
};

struct NodeArithmetic : NodeStatement {
    std::vector<std::unique_ptr<NodeExpression>> reversepolish;

    NodeArithmetic(std::vector<std::unique_ptr<NodeExpression>> rpn)
        : reversepolish(std::move(rpn)) {}
};

struct NodeReturn : NodeStatement {
    std::unique_ptr<NodeArithmetic> rpn;

    NodeReturn(std::unique_ptr<NodeArithmetic> r)
        : rpn(std::move(r)) {}
};

//---> Expression ∈ {Integer, Arithmetic, FunctionCall}
struct NodeExpression {
    virtual ~NodeExpression() = default;
};

struct NodeInteger : NodeExpression {
    Token value;

    NodeInteger(Token v)
        : value(std::move(v)) {}
};

struct NodeIdentifier : NodeExpression {
    Token value;

    NodeIdentifier(Token v)
        : value(std::move(v)) {}
};

struct NodeOperator : NodeExpression {
    Token value;

    NodeOperator(Token v)
        : value(std::move(v)) {}
};

struct NodeBoolean : NodeExpression {
    Token value;

    NodeBoolean(Token v)
        : value(std::move(v)) {}
};

struct NodeFunctionCall : NodeExpression {
    Token value;
    std::vector<Token> inputs; // int_lit or identifier

    NodeFunctionCall(Token v, std::vector<Token> i)
        : value(std::move(v)), inputs(std::move(i)) {}
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