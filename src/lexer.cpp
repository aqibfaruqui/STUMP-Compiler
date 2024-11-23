#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include "lexer.h"


Lexer::Lexer(const std::string& src) 
    : m_src(std::move(src)), m_idx(0) {}

std::vector<Token> Lexer::tokenise() {
    std::vector<Token> tokens;
    std::string buffer;

    while (peek().has_value()) {

        /* SKIP WHITESPACE */
        if (std::isspace(peek().value())) consume();

        /* SKIP COMMENTS */
        else if (peek().value() == '/' && peek(1).has_value() && peek(1).value() == '/') {
            while (peek().has_value() && peek().value() != '\n') {
                consume();
            }
        }
            
        /* INT_LIT & FLOAT_LIT */
        else if (std::isdigit(peek().value())) {
            buffer.push_back(consume());
            while (peek().has_value() && std::isdigit(peek().value())) {
                buffer.push_back(consume());
            }
            if (std::isspace(peek().value()) || peek().value() == ';') {
                tokens.push_back({.type = TokenType::INT_LIT, .value = buffer});
                buffer.clear();
            } else {
                std::cerr << "invalid integer" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        /* TEXT-BASED TOKENS */
        else if (std::isalpha(peek().value())) {
            buffer.push_back(consume());
            while (peek().has_value() && std::isalnum(peek().value())) {
                buffer.push_back(consume());
            }

            static const std::unordered_map<std::string, TokenType> text_tokens = {
                {"return", TokenType::RETURN},
                {"if", TokenType::IF},
                {"while", TokenType::WHILE},
                {"else", TokenType::ELSE},
                {"function", TokenType::FUNCTION},
                {"int", TokenType::INT},
                {"float", TokenType::FLOAT},
                {"setLED", TokenType::SET_LED},
                {"clearLED", TokenType::CLEAR_LED},
                {"setCursor", TokenType::SET_CURSOR},
                {"printLCD", TokenType::PRINT_LCD},
                {"readKey", TokenType::READ_KEY},
                {"waitKey", TokenType::WAIT_KEY},
                {"playSound", TokenType::PLAY_SOUND},
                {"stopSound", TokenType::STOP_SOUND},
                {"delay", TokenType::DELAY},
                {"asr", TokenType::SHIFT_ASR},
                {"ror", TokenType::SHIFT_ROR},
                {"rrc", TokenType::SHIFT_RRC}
            };

            auto text_token = text_tokens.find(buffer);
            if (text_token != text_tokens.end()) {
                tokens.push_back({.type = text_token->second});
            } else {
                tokens.push_back({.type = TokenType::IDENTIFIER, .value = buffer});
            }
            buffer.clear();
        }

        /* SINGLE CHARACTER TOKENS */
        else {
            char c = peek().value();
            switch(c) {
                case ';': tokens.push_back({.type = TokenType::SEMI}); break;
                case '(': tokens.push_back({.type = TokenType::LBRACKET}); break;
                case ')': tokens.push_back({.type = TokenType::RBRACKET}); break;
                case '{': tokens.push_back({.type = TokenType::LBRACE}); break;
                case '}': tokens.push_back({.type = TokenType::RBRACE}); break;
                case ',': tokens.push_back({.type = TokenType::COMMA}); break;
                case '+': tokens.push_back({.type = TokenType::PLUS}); break;
                case '-': tokens.push_back({.type = TokenType::MINUS}); break;
                case '*': tokens.push_back({.type = TokenType::MULTIPLY}); break;
                case '/': tokens.push_back({.type = TokenType::DIVIDE}); break;
                case '&': tokens.push_back({.type = TokenType::BIT_AND}); break;
                case '|': tokens.push_back({.type = TokenType::BIT_OR}); break;
                case '=':
                    if (peek(1).has_value() && peek(1).value() == '=') {
                        consume();
                        tokens.push_back({.type = TokenType::EQUALS});
                    } else {
                        tokens.push_back({.type = TokenType::ASSIGN});
                    }
                    break;
                case '<':
                    if (peek(1).has_value() && peek(1).value() == '=') {
                        consume();
                        tokens.push_back({.type = TokenType::LESS_EQUAL});
                    } else {
                        tokens.push_back({.type = TokenType::LESS});
                    }
                    break;
                case '>':
                    if (peek(1).has_value() && peek(1).value() == '=') {
                        consume();
                        tokens.push_back({.type = TokenType::GREATER_EQUAL});
                    } else {
                        tokens.push_back({.type = TokenType::GREATER});
                    }
                    break;
                default:
                    std::cerr << "invalid character" << std::endl;
                    tokens.push_back({.type = TokenType::INVALID}); 
                    break;
            }
            consume();
        }
    }
    m_idx = 0;
    buffer.clear();
    tokens.push_back({.type = TokenType::END_OF_FILE});
    return tokens;
}

std::optional<char> Lexer::peek(int ahead) const {
    if (m_idx + ahead >= m_src.length()) {
        return {};
    } 
    return m_src.at(m_idx + ahead);    
}

char Lexer::consume() {
    return m_src.at(m_idx++);
}