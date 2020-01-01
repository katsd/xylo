//
// parser.hpp
// xylo
//
// Copyright (c) 2020 Katsu Matsuda. All rights reserved.
//

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <stdio.h>
#include <vector>
#include <string>

#include "lexer.hpp"
#include "token.hpp"
#include "vm/vm.hpp"

class Parser
{
private:
    std::string code_str;

    std::vector<Token> code;

    std::vector<VM::Obj> const_table;

public:
    Parser(std::string code_str)
    {
        this->code_str = code_str;
    }

    struct Result
    {
        bool success;

        std::vector<unsigned long> iseq;

        std::vector<VM::Obj> const_table;

        Result(bool success, std::vector<unsigned long> iseq, std::vector<VM::Obj> const_table)
        {
            this->success = success;
            this->iseq = iseq;
            this->const_table = const_table;
        }
    };

    Result Parse();
};

#endif /* PARSER_HPP_ */
