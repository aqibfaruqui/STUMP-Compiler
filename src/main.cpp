#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "lexer.h"
#include "parser.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage should be..." << std::endl;
        std::cerr << "./src/main <input.stump>" << std::endl;
        exit(EXIT_FAILURE);
    }

    /* Reading whole input file into string */
    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    // TOKENISE
    Lexer lexer(std::move(contents));
    std::vector<Token> tokens = lexer.tokenise();

    // PARSE

    std::cout << "success" << std::endl;

    return EXIT_SUCCESS;
}