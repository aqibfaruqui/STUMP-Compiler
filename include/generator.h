#ifndef GENERATOR_H
#define GENERATOR_H

#include <sstream>
#include "ast_visitor.h"
#include "parser.h"

class Generator : public ASTVisitor {
public:
    Generator();
    
    std::string generate(const NodeProgram& program);
    
    // Statement visitors
    void visit(const NodeVarDecl& node) override;
    void visit(const NodeArithmetic& node) override;
    void visit(const NodeAssignment& node) override;
    void visit(const NodeReturn& node) override;
    
    // Expression visitors
    void visit(const NodeInteger& node) override;
    void visit(const NodeBoolean& node) override;
    void visit(const NodeIdentifier& node) override;
    void visit(const NodeOperator& node) override;
    void visit(const NodeFunctionCall& node) override;

private:
    void generateFunction(const NodeFunction& func);
    
    std::stringstream m_output;
    size_t m_stackOffset = 0;
};

#endif