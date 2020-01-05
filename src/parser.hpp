//
// parser.hpp
// xylo
//
// Copyright (c) 2020 Katsu Matsuda. All rights reserved.
//

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <stdio.h>
#include <map>
#include <vector>
#include <set>
#include <string>

#include "lexer.hpp"
#include "token.hpp"
#include "vm/vm.hpp"

class Parser
{
private:
    enum NodeType
    {
        ROOT,
        BLOCK,
        DEF_FUNC,
        FUNC,
        VAR,
        CONST,
        ASSIGN,
        UOPERATOR,
        BOPERATOR,
        RETURN,
        REPEAT,
        FOR,
        WHILE,
        IF,
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
            this->type = NodeType::ROOT;
            this->token = Token();
            this->child = std::vector<Node>();
        }

        void Out(unsigned long long indent_size, std::vector<VM::Obj> &const_table);
    };

    struct ParseResult
    {
        bool success;

        Node node;

        unsigned long idx;

        ParseResult(bool success)
        {
            this->success = success;
            this->node = Node();
            this->idx = 0;
        }

        ParseResult(Node node, unsigned long idx)
        {
            this->success = true;
            this->node = node;
            this->idx = idx;
        }

        ParseResult(bool success, Node node, unsigned long idx)
        {
            this->success = success;
            this->node = node;
            this->idx = idx;
        }
    };

    std::string code_str;

    std::vector<Token> code;

    std::vector<unsigned long> iseq;

    std::vector<VM::Obj> const_table;

    Node ast;

    std::map<unsigned long, bool> var_decleared;

    unsigned long code_size;

    const std::vector<std::set<Symbol>> operator_rank = {
        {Symbol::OR},
        {Symbol::AND},
        {Symbol::BOR},
        {Symbol::BXOR},
        {Symbol::BAND},
        {Symbol::EQUAL, Symbol::NEQUAL},
        {Symbol::LESS, Symbol::LESSEQ, Symbol::GRE, Symbol::GREEQ},
        {Symbol::PLUS, Symbol::MINUS},
        {Symbol::MUL, Symbol::DIV, Symbol::MOD},
    };

    bool GenerateAST();

    ParseResult ParseStatement(unsigned long idx);

    ParseResult ParseExpression(unsigned long idx);

    ParseResult ParseExpression(unsigned long idx, unsigned long rank);

    ParseResult ParseTerm(unsigned long idx);

    ParseResult ParseVariable(unsigned long idx);

    bool GenerateIseq();

    bool GenerateInst(Node node, const Node &par);

    inline void PushInst(unsigned long inst)
    {
        iseq.push_back(inst);
    }

    bool PushVar(const unsigned long var_address, const std::string &var_name);

    inline bool CompReserved(Token token, Reserved reserved)
    {
        return token.type == TokenType::RESERVED && token.token.reserved == reserved;
    }

    inline bool CompSymbol(Token token, Symbol symbol)
    {
        return token.type == TokenType::SYMBOL && token.token.symbol == symbol;
    }

    inline ParseResult FailedToParse(unsigned long idx, std::string msg)
    {
        printf("#%ld : %s\n", idx, msg.c_str());
        return ParseResult(false);
    }

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
