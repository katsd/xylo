//
// lexer.hpp
// xylo
//
// Copyright (c) 2020 Katsu Matsuda. All rights reserved.
//

#ifndef LEXER_CPP_
#define LEXER_CPP_

#include <stdio.h>
#include <string>
#include <vector>
#include <map>

#include "token.hpp"
#include "vm/vm.hpp"

class Lexer
{
private:
    bool success;

    std::string code_str;

    std::vector<Token> code;

    std::vector<VM::Obj> const_table;

    unsigned long const_num;

    std::map<std::string, unsigned long> const_map;

    unsigned long other_token_num;

    std::map<std::string, unsigned long> other_token_map;

    std::string RemoveComment(std::string code_str);

    inline void PushReserved(Reserved token, std::string &current)
    {
        code.push_back(Token(TokenType::RESERVED, TokenVal(token), ""));
        current = "";
    }

    inline void PushSymbol(Symbol token, std::string &current)
    {
        PushToken(current);
        code.push_back(Token(TokenType::SYMBOL, TokenVal(token), ""));
    }

    void PushToken(std::string &token);

    long Str2Int(std::string str);

    double Str2Float(std::string str);

    void OutCode();

    void OutConstTable();

public:
    struct Result
    {
        bool success;

        std::vector<Token> code;

        std::vector<VM::Obj> const_table;

        Result(bool success, std::vector<Token> code, std::vector<VM::Obj> const_table)
        {
            this->success = success;
            this->code = code;
            this->const_table = const_table;
        }
    };

    Result Tokenize();

    Lexer(std::string code_str)
    {
        this->code_str = code_str;
    }
};

#endif /* LEXER_CPP_ */
