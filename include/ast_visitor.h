#ifndef AST_VISITOR_H
#define AST_VISITOR_H

struct NodeVarDecl;
struct NodeArithmetic;
struct NodeAssignment;
struct NodeReturn;
struct NodeInteger;
struct NodeBoolean;
struct NodeIdentifier;
struct NodeOperator;
struct NodeFunctionCall;

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    // Statement visitors
    virtual void visit(const NodeVarDecl& node) = 0;
    virtual void visit(const NodeArithmetic& node) = 0;
    virtual void visit(const NodeAssignment& node) = 0;
    virtual void visit(const NodeReturn& node) = 0;
    
    // Expression visitors
    virtual void visit(const NodeInteger& node) = 0;
    virtual void visit(const NodeBoolean& node) = 0;
    virtual void visit(const NodeIdentifier& node) = 0;
    virtual void visit(const NodeOperator& node) = 0;
    virtual void visit(const NodeFunctionCall& node) = 0;
};

#endif