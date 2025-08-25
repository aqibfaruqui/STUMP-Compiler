#include "generator.h"

Generator::Generator() = default;

std::string Generator::generate(const NodeProgram& program) {
    m_output << "ORG 0\n";
    m_output << "B main\n";
    m_output << "SP     EQU     R6\n";
    m_output << "stack  DATA    0x1200\n\n";
    
    for (const auto& func : program.functions) {
        generateFunction(*func);
    }
    
    return m_output.str();
}

void Generator::generateFunction(const NodeFunction& func) {
    m_output << func.name << ":\n";
    m_output << "MOV R1, #0\n";
    m_output << "MOV R2, #0\n";
    m_output << "MOV R3, #0\n";
    m_output << "MOV R4, #0\n";
    m_output << "MOV R5, #0\n";
    m_output << "LD SP, [R0, #stack]\n";
    
    m_stackOffset = 0;
    
    for (const auto& stmt : func.body->statements) {
        stmt->accept(*this);
    }
}

// Statement visitors
void Generator::visit(const NodeVarDecl& node) {
    node.rpn->accept(*this);
    
    m_output << "ST R1, [SP]\n";
    m_output << "ADD SP, SP, #1\n";
    m_stackOffset++;
}

void Generator::visit(const NodeArithmetic& node) {
    for (const auto& expr : node.reversepolish) {
        expr->accept(*this);
    }
}

void Generator::visit(const NodeAssignment& node) {
    // TODO: Implement assignment generation
}

void Generator::visit(const NodeReturn& node) {
    // TODO: Implement return generation
}

// Expression visitors
void Generator::visit(const NodeInteger& node) {
    m_output << "LD R1, [PC, #1]\n";
    m_output << "ADD PC, PC, #1\n";
    m_output << "DEFW " << node.value.value.value() << "\n";
}

void Generator::visit(const NodeBoolean& node) {
    m_output << "LD R1, [PC, #1]\n";
    m_output << "ADD PC, PC, #1\n";
    if (node.value.type == TokenType::TRUE) {
        m_output << "DEFW 1\n";
    } else {
        m_output << "DEFW 0\n";
    }
}

void Generator::visit(const NodeIdentifier& node) {
    // TODO: Implement identifier generation
}

void Generator::visit(const NodeOperator& node) {
    // TODO: Implement operator generation
}

void Generator::visit(const NodeFunctionCall& node) {
    // TODO: Implement function call generation
}