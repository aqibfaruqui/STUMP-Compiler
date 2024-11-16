#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <optional>


struct NodeFunction { };

struct NodeExpr { };

struct NodeReturn { };


class Parser {
public:
    /* Constructor */
    explicit Parser(const std::vector<Token>& tokens);

    /* Parsing input tokens */
    std::optional<NodeReturn> parse();

private:
    /* Looking ahead n tokens */
    [[nodiscard]] inline std::optional<Token> peek(int ahead = 0) const;

    /* Consuming next token */
    inline Token consume();

    const std::vector<Token> m_tokens;
    size_t m_idx = 0;
};

#endif