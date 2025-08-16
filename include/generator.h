#ifndef GENERATOR_H
#define GENERATOR_H

#include <sstream>
#include "parser.h"

class Generator {
public:
    /* Constructor */
    explicit Generator(std::unique_ptr<NodeProgram> program);
    
    /* Parsing abstract syntax tree from parser */
    std::string generate();

private:
    // TODO: Add generation methods

    const std::unique_ptr<NodeProgram> m_program;
    size_t m_linenumber;
    std::stringstream m_output;
};

#endif