#include <iostream>
#include <fstream>
#include "generator.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage should be..." << std::endl;
        std::cerr << "./src/main <input.stump>" << std::endl;
        exit(EXIT_FAILURE);
    }

    /* Reading input file into string */
    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    std::cout << "starting" << std::endl;

    //---> TOKENISE
    // Lexer lexer(contents);
    // std::vector<Token> tokens = lexer.tokenise();

    std::cout << "successful lexing, now parsing" << std::endl;

    //---> PARSE
    // Parser parser(tokens);
    // std::unique_ptr<NodeProgram> program = parser.parse();

    std::cout << "successful parsing, now generating" << std::endl;

    //---> GENERATE
    std::unique_ptr<NodeProgram> program;
    Generator generator(std::move(program));
    std::string output = generator.generate();

    std::cout << "code generated" << std::endl;

    /* Writing string into output file */
    std::ofstream out("../samples/output.s");
    out << output;
    
    return EXIT_SUCCESS;
}