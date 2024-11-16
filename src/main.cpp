#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "lexer.h"
#include "parser.h"

/*
 * Input: program file
 * Output: vector of tokens
 * Ensuring tokens start with a letter?
 */
std::vector<Token> tokenise(const std::string& str) {
    std::vector<Token> tokens;
    std::string buffer;

    for (int i = 0; i < str.length(); i++) {
        char c = str.at(i);
        
        /* INT_LIT */
        if (std::isdigit(c)) {
            buffer.push_back(c);
            i++;
            while (std::isdigit(str.at(i))) {
                buffer.push_back(str.at(str.at(i)));
                i++;
            } 
            if (!(std::isspace(str.at(i))) && !(str.at(i) == ';')) {
                std::cerr << "invalid integer" << std::endl;
                exit(EXIT_FAILURE);
            }
            i--;
            tokens.push_back({.type = TokenType::INT_LIT, .value = buffer});
            buffer.clear();
            continue;
        }

        /* RETURN */
        else if (std::isalpha(c)) {
            buffer.push_back(c);
            i++;
            while (std::isalnum(str.at(i))) {
                buffer.push_back(str.at(i));
                i++;
            }
            i--;
            if (buffer == "return") {
                tokens.push_back({.type = TokenType::RETURN});
                buffer.clear();
                continue;
            } else {
                std::cerr << "not return" << std::endl;
                exit(EXIT_FAILURE);
            }
        } 

        /* SEMI */
        else if (c == ';') {
            tokens.push_back({.type = TokenType::SEMI});
            continue;
        } 

        else if (std::isspace(c)) continue;
    }
    return tokens;
}

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
    std::vector<Token> tokens = tokenise(contents);

    // PARSE

    std::cout << "success" << std::endl;

    return EXIT_SUCCESS;
}