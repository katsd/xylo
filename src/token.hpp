//
// token.hpp
// xylo
//
// Copyright (c) 2020 Katsu Matsuda. All rights reserved.
//

#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include <string>

enum Reserved
{
    FUNC,
    RETURN,
    REPEAT,
    FOR,
    WHILE,
    IF,
    ELSE,
};

enum Symbol
{
    PLUS,   // +
    MINUS,  // -
    MUL,    // *
    DIV,    // /
    MOD,    //%
    BAND,   // &
    BOR,    //|
    BXOR,   // ^
    BNOT,   // ~
    AND,    // &&
    OR,     // ||
    NOT,    // !
    EQUAL,  // ==
    NEQUAL, // !=
    GRE,    // >
    GREEQ,  // >=
    LESS,   // <
    LESSEQ, // <=

    ASSIGN,   // =
    QUE,      // ?
    COMMMA,   // ,
    DOT,      // .
    LPAREN,   // (
    RPAREN,   // )
    LBRACKET, // {
    RBRACKET, // }

};

enum TokenType
{
    RESERVED,
    SYMBOL,
    CONST,
    OTHER,
};

union TokenVal {
    Reserved reserved;
    Symbol symbol;
    unsigned long val;

    TokenVal() {}

    TokenVal(Reserved reserved)
    {
        this->reserved = reserved;
    }

    TokenVal(Symbol symbol)
    {
        this->symbol = symbol;
    }

    TokenVal(unsigned long val)
    {
        this->val = val;
    }
};

struct Token
{
    TokenType type;
    TokenVal token;
    std::string str;

    Token(TokenType type, TokenVal token, std::string str)
    {
        this->type = type;
        this->token = token;
        this->str = str;
    }

    Token()
    {
        this->type = TokenType::OTHER;
        this->token = TokenVal(0);
        this->str = "";
    }
};

#endif /* TOKEN_HPP_ */
