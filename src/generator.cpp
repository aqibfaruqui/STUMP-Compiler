/* function main() {
    int x = 4;
    int y = 5;
    int z = x + y;
    return z;
   } 

    - program->functions has one function, main
    - after init stack, call generateMain() -> init registers and checks main exists 
    - call generateFunction() for func : functions
    - ...

    known problems:
        - if we generate main first, function calls from main must write missing function addresses to PC
        - problem persists for any function that calls others
        - therefore, pseudo topologically sorting our functions could let us write functions in order of calling
            - problem of circular function calling? may be 'correct' if calls depend on conditional statements

        - otherwise, let generator write functions with memory address left blank and let a second pass through fill them in
            - how to edit std::stringstream on second pass through??

        +++ treat like C, function calls only find functions declared above it!
            - also does mean to generate main last
*/

#include "generator.h"

Generator::Generator(std::unique_ptr<NodeProgram> program)
    : m_program(std::move(program)) {}

std::string Generator::generate() {
    m_output << "ORG 0\n";
    m_output << "B main\n";
    m_output << "SP     EQU     R6\n";
    m_output << "stack  DATA    0x1200\n\n";
    generateMain();
    for (auto& func : m_program->functions) {
        generateFunction(std::make_unique<NodeFunction>(func));
    }
    return m_output.str();
}

void Generator::generateMain() {
    bool foundmain = false;
    std::unique_ptr<NodeFunction> main;
    for (auto& func : m_program->functions) {
        if (func->name == "main") {
            foundmain = true;
            main = std::make_unique<NodeFunction>(func);
            break;
        }        
    }
    if (!foundmain) {
        std::cerr << "no main function" << std::endl;
        exit(EXIT_FAILURE);
    }

    m_output << "main:\n";
    m_output << "MOV R1, #0\n";
    m_output << "MOV R2, #0\n";
    m_output << "MOV R3, #0\n";
    m_output << "MOV R4, #0\n";
    m_output << "MOV R5, #0\n";
    m_output << "LD SP, [R0, #stack]\n";

    // func->name           ; ignore for main
    // func->parameters     ; ignore for main
    // func->body           ; generateBody()? body->statements

    // for (auto& stmt : main->body) {
    //     continue;
    // }

    return;
}

void Generator::generateFunction(std::unique_ptr<NodeFunction> function) {
    return;
}