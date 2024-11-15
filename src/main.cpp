#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"

// struct Token {
    
// }

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage should be..." << std::endl;
        std::cerr << "stumpc <input.stump>" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "hello world!" << std::endl;

    /* Reading whole input file into string */
    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    

    return EXIT_SUCCESS;
}