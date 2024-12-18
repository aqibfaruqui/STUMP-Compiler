#include "parser.h"

// ============================= Constructing & Entering =============================

// Constructor
Parser::Parser(std::vector<Token>& tokens)
    : m_tokens(std::move(tokens)) {}

// Entry to parser
std::unique_ptr<NodeProgram> Parser::parse() {
    auto program = std::make_unique<NodeProgram>();

    while (!atEnd()) {
        if (check(TokenType::INT)) {
            program->globals.push_back(parseVarDecl(true));
        }
        program->functions.push_back(parseFunction());
    }
    return program;
}


// ====================================== Parser ======================================

// Function parser
std::unique_ptr<NodeFunction> Parser::parseFunction() {
    /* function name(... */
    consume(TokenType::FUNCTION);
    auto name = consume(TokenType::IDENTIFIER).value;
    consume(TokenType::LBRACKET);

    /* param1, param2)...*/
    std::vector<std::string> parameters;
    if (!check(TokenType::RBRACKET)) {
        do {
            parameters.push_back(consume(TokenType::IDENTIFIER).value.value());
        } while (checkAdvance(TokenType::COMMA));
    }
    consume(TokenType::RBRACKET);

    /* { (body) ... */
    consume(TokenType::LBRACE);
    auto body = parseBody(); 

    /* Function: name (parameters) {body} */
    return std::make_unique<NodeFunction>(name.value(), std::move(parameters), std::move(body));
}

// Function Body parser
std::unique_ptr<NodeBody> Parser::parseBody() {
    std::vector<std::unique_ptr<NodeStatement>> statements;
    /* ... stmt1; stmt2; }*/
    while (!check(TokenType::RBRACE)) {
        statements.push_back(parseStatement());
    }

    consume(TokenType::RBRACE);
    return std::make_unique<NodeBody>(std::move(statements));
}

// Statement parser (one line of a body)
std::unique_ptr<NodeStatement> Parser::parseStatement() {
    auto stmt = std::make_unique<NodeStatement>();
    
    TokenType t = peek().type;
    while (!check(TokenType::SEMI)) {
        switch (t) {
        case TokenType::IDENTIFIER: return parseAssignment(); break;
        case TokenType::INT:        return parseVarDecl(); break;
        case TokenType::RETURN:     return parseReturn(); break;
        // case if/while
        // case FPGA peripherals (make libraries to include?!)
        default: break; // error? invalid statement start
        }
    }
    return nullptr; // placeholder for error case
}

// Assignment parser 
std::unique_ptr<NodeAssignment> Parser::parseAssignment() {
    std::string name = consume(TokenType::IDENTIFIER).value.value();
    consume(TokenType::ASSIGN);
    std::unique_ptr<NodeArithmetic> expr = parseArithmetic();

    return std::make_unique<NodeAssignment>(name, expr);
}

// Variable declaration parser
std::unique_ptr<NodeVarDecl> Parser::parseVarDecl(bool global = false) {    
    consume(TokenType::INT);
    std::string name = consume(TokenType::IDENTIFIER).value.value();
    consume(TokenType::ASSIGN);
    std::unique_ptr<NodeArithmetic> expr = parseArithmetic();

    return std::make_unique<NodeVarDecl>(name, expr, global);
}

// Arithmetic parser (Shunting-Yard algorithm)
std::unique_ptr<NodeArithmetic> Parser::parseArithmetic() {
    std::queue<Token> output;    // int/iden/func
    std::stack<char> operators;

    while (!check(TokenType::SEMI)) {
        if (check(TokenType::INT_LIT)) {
            continue;
        }
    }

    consume(TokenType::SEMI);
    return 0;
}

std::unique_ptr<NodeReturn> Parser::parseReturn() {
    consume(TokenType::RETURN);
    return std::make_unique<NodeReturn>(parseArithmetic()->result); 
}

// ============================= Arithmetic Helper Methods =============================

bool Parser::isOperator(TokenType type) {
    return type == TokenType::PLUS ||
           type == TokenType::MINUS ||
           type == TokenType::MULTIPLY ||
           type == TokenType::DIVIDE;
}

char Parser::getOperator(TokenType op) {
    switch (op) {
        case TokenType::PLUS: return '+';
        case TokenType::MINUS: return '-';
        case TokenType::MULTIPLY: return '*';
        case TokenType::DIVIDE: return '/';
        default: break;
    }
}

int Parser::getPrecedence(char op) {
    switch (op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        default: return 0;
    }
}


// ================================== Token Management =================================

Token Parser::peek() const {
    return m_tokens.at(m_idx);
}

Token Parser::consume(TokenType type) {
    if (check(type)) return advance();
    throw error(peek());
}

Token Parser::advance() {
    if (!atEnd()) return m_tokens.at(m_idx++);
    return m_tokens.at(m_idx);
}

bool Parser::check(TokenType type) {
    if (atEnd() || peek().type != type) return false;
    return true;
}

bool Parser::checkAdvance(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::atEnd() {
    return peek().type == TokenType::END_OF_FILE;
}



// ==================================== Error Handling ====================================

std::runtime_error Parser::error(const Token& token) {
    // include token name -> error handling
    std::string error_message;
    return std::runtime_error(error_message);
}
