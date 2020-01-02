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
    enum NodeType
    {
        Expression,
        Statement,
        Root,
    };

    struct Node
    {
        NodeType type;

        Token token;

        std::vector<Node> child;

        Node(NodeType type, Token token)
        {
            this->type = type;
            this->token = token;
            this->child = std::vector<Node>();
        }

        Node()
        {
            this->type = NodeType::Root;
            this->token = Token();
            this->child = std::vector<Node>();
        }
    };

    std::string code_str;

    std::vector<Token> code;

    std::vector<unsigned long> iseq;

    std::vector<VM::Obj> const_table;

    Node ast;

    bool GenerateAST();

    bool GenerateIseq();

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
