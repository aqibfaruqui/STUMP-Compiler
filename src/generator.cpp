#include "generator.h"

Generator::Generator(std::unique_ptr<NodeProgram> program)
    : m_program(std::move(program)) {}

std::string Generator::generate() {
    m_output << "ORG 0\n";
    m_output << "B main\n\n";
    m_output << "main:\n";
    m_output << "    LD R6, [PC, #1]\n";
    m_output << "    ADD R6, R6, #1\n";
    m_output << "    DEFW 0x1000\n";

    

    return m_output.str();
}