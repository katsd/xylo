//
// parser.cpp
// xylo
//
// Copyright (c) 2020 Katsu Matsuda. All rights reserved.
//

#include "parser.hpp"

#define STR(var) #var

Parser::Result Parser::Parse()
{
    auto lexer_res = Lexer(code_str).Tokenize();

    if (!lexer_res.success)
    {
        printf("failed to tokenize\n");
        return Result(false, iseq, const_table);
    }

    code = lexer_res.code;

    const_table = lexer_res.const_table;

    GenerateAST();

    GenerateIseq();

    return Result(true, iseq, const_table);
}

bool Parser::GenerateAST()
{
    ast = Node();

    code_size = code.size();

    unsigned long idx = 0;

    while (idx < code_size)
    {
        ParseResult res = ParseStatement(idx);

        if (res.success)
        {
            idx = res.idx;
            ast.child.push_back(res.node);
        }
        else
        {
            printf("failed to parse\n");
            return false;
        }
    }

    ast.Out(0, const_table);

    return true;
}

Parser::ParseResult Parser::ParseStatement(unsigned long idx)
{
    Node node;

    if (idx >= code_size)
        return FailedToParse(idx, "expected statement");

    switch (code[idx].type)
    {
    case TokenType::RESERVED:
    {
        switch (code[idx].token.reserved)
        {
        case Reserved::FUNC:
        {
            node.type = NodeType::DEF_FUNC;
            node.token = code[idx];

            if (idx + 1 < code_size && CompSymbol(code[idx + 1], Symbol::LPAREN))
            {
                idx += 2;

                if (CompSymbol(code[idx], Symbol::RPAREN))
                {
                    idx += 1;
                }
                else
                {
                    bool ok = false;

                    while (idx < code_size)
                    {
                        auto res = ParseExpression(idx);

                        if (res.success)
                        {
                            idx = res.idx;
                            node.child.push_back(res.node);
                        }
                        else
                        {
                            return ParseResult(false);
                        }

                        if (CompSymbol(code[idx], Symbol::RPAREN))
                        {
                            idx += 1;
                            ok = true;
                            break;
                        }

                        if (CompSymbol(code[idx], Symbol::COMMMA))
                        {
                            idx += 1;
                        }
                    }

                    if (!ok)
                        return FailedToParse(idx, "expected ')'");
                }

                if (idx < code_size && CompSymbol(code[idx], Symbol::LBRACKET))
                {
                    auto res = ParseStatement(idx);

                    if (res.success)
                    {
                        idx = res.idx;
                        node.child.push_back(res.node);
                    }
                    else
                    {
                        return ParseResult(false);
                    }
                }
                else
                {
                    return FailedToParse(idx, "expected '{'");
                }
            }
            else
            {
                return FailedToParse(idx, "expected '('");
            }
        }
        break;

        case Reserved::RETURN:
        {
            node.type = NodeType::RETURN;
            node.token = code[idx];

            idx += 1;

            auto res = ParseExpression(idx);

            if (res.success)
            {
                idx = res.idx;
                node.child.push_back(res.node);
            }
            else
            {
                return ParseResult(false);
            }
        }
        break;

        case Reserved::FOR:
        {
            node.type = NodeType::FOR;
            node.token = code[idx];

            idx += 1;
            if (idx < code_size && CompSymbol(code[idx], Symbol::LPAREN))
            {
                idx += 1;

                {
                    auto res = ParseVariable(idx);

                    if (res.success)
                    {
                        idx = res.idx;
                        node.child.push_back(res.node);
                    }
                    else
                    {
                        return ParseResult(false);
                    }
                }

                if (!(idx < code_size && CompSymbol(code[idx], Symbol::COMMMA)))
                    return FailedToParse(idx, "expected ','");

                idx += 1;

                {
                    auto res = ParseExpression(idx);

                    if (res.success)
                    {
                        idx = res.idx;
                        node.child.push_back(res.node);
                    }
                    else
                    {
                        return ParseResult(false);
                    }
                }

                if (!(idx < code_size && CompSymbol(code[idx], Symbol::RPAREN)))
                    return FailedToParse(idx, "expected ')'");

                idx += 1;

                {
                    auto res = ParseStatement(idx);

                    if (res.success)
                    {
                        idx = res.idx;
                        node.child.push_back(res.node);
                    }
                    else
                    {
                        return ParseResult(false);
                    }
                }
            }
            else
            {
                return FailedToParse(idx, "expected '('");
            }
        }
        break;

        case Reserved::REPEAT:
        case Reserved::WHILE:
        {
            if (code[idx].token.reserved == Reserved::REPEAT)
                node.type = NodeType::REPEAT;
            else
                node.type = NodeType::WHILE;

            node.token = code[idx];

            idx += 1;

            {
                auto res = ParseExpression(idx);

                if (res.success)
                {
                    idx = res.idx;
                    node.child.push_back(res.node);
                }
                else
                {
                    return ParseResult(false);
                }
            }

            {
                auto res = ParseStatement(idx);

                if (res.success)
                {
                    idx = res.idx;
                    node.child.push_back(res.node);
                }
                else
                {
                    return ParseResult(false);
                }
            }
        }
        break;

        case Reserved::IF:
        {
            node.type = NodeType::IF;
            node.token = code[idx];

            idx += 1;

            if (!(idx < code_size && CompSymbol(code[idx], Symbol::LPAREN)))
                return FailedToParse(idx, "expected '('");

            idx += 1;

            {
                auto res = ParseExpression(idx);

                if (res.success)
                {
                    idx = res.idx;
                    node.child.push_back(res.node);
                }
                else
                {
                    return ParseResult(false);
                }
            }

            if (!(idx < code_size && CompSymbol(code[idx], Symbol::RPAREN)))
                return FailedToParse(idx, "expected ')'");

            idx += 1;

            {
                auto res = ParseStatement(idx);

                if (res.success)
                {
                    idx = res.idx;
                    node.child.push_back(res.node);
                }
                else
                {
                    return ParseResult(false);
                }
            }

            if (idx < code_size && CompReserved(code[idx], Reserved::ELSE))
            {
                idx += 1;
                auto res = ParseStatement(idx);

                if (res.success)
                {
                    idx = res.idx;
                    node.child.push_back(res.node);
                }
                else
                {
                    return ParseResult(false);
                }
            }
        }
        break;

        default:
            break;
        }
    }
    break;

    case TokenType::SYMBOL:
    {
        node.token = code[idx];

        bool ok = false;

        if (code[idx].token.symbol == Symbol::LBRACKET)
        {
            node.type = NodeType::BLOCK;

            idx += 1;

            if (idx < code_size && CompSymbol(code[idx], Symbol::RBRACKET))
            {
                idx += 1;
                ok = true;
                break;
            }

            while (idx < code_size)
            {
                auto res = ParseStatement(idx);

                if (res.success)
                {
                    idx = res.idx;
                    node.child.push_back(res.node);
                }
                else
                {
                    return ParseResult(false);
                }

                if (idx < code_size && CompSymbol(code[idx], Symbol::RBRACKET))
                {
                    idx += 1;
                    ok = true;
                    break;
                }
            }
        }

        if (!ok)
            return FailedToParse(idx, "expected '}'");
    }
    break;

    case TokenType::CONST:
    {
        return FailedToParse(idx, "expected statement");
    }
    break;

    case TokenType::OTHER:
    {
        if (idx + 1 < code_size && code[idx + 1].type == TokenType::SYMBOL)
        {
            switch (code[idx + 1].token.symbol)
            {
            case Symbol::ASSIGN:
            {
                node.type = NodeType::ASSIGN;
                node.token = code[idx + 1];

                {
                    auto res = ParseVariable(idx);

                    if (res.success)
                    {
                        node.child.push_back(res.node);
                    }
                    else
                    {
                        return ParseResult(false);
                    }
                }

                idx += 2;

                {
                    auto res = ParseExpression(idx);

                    if (res.success)
                    {
                        idx = res.idx;
                        node.child.push_back(res.node);
                    }
                    else
                    {
                        return ParseResult(false);
                    }
                }

                return ParseResult(true, node, idx);
            }
            break;

            case Symbol::LPAREN:
            {
                auto res = ParseExpression(idx);

                if (res.success)
                {
                    idx = res.idx;
                    node = res.node;
                }
                else
                {
                    return ParseResult(false);
                }

                return ParseResult(true, node, idx);
            }
            break;

            default:
                break;
            }
        }

        auto res = ParseExpression(idx);

        if (res.success)
        {
            idx = res.idx;
            node = res.node;
        }
        else
        {
            return ParseResult(false);
        }
    }
    break;

    default:
        return FailedToParse(idx, "expected statement");
    }

    return ParseResult(node, idx);
}

Parser::ParseResult Parser::ParseExpression(unsigned long idx)
{
    return ParseExpression(idx, 0);
}

Parser::ParseResult Parser::ParseExpression(unsigned long idx, unsigned long rank)
{
    if (idx >= code_size)
        return FailedToParse(idx, "expected expression");

    if (rank >= operator_rank.size())
        return ParseTerm(idx);

    Node node;

    auto left = ParseExpression(idx, rank + 1);

    if (!left.success)
        return ParseResult(false);

    idx = left.idx;

    if (idx < code_size && code[idx].type == TokenType::SYMBOL && operator_rank[rank].count(code[idx].token.symbol) > 0)
    {
        node.type = NodeType::BOPERATOR;
        node.token = code[idx];

        idx += 1;

        auto right = ParseExpression(idx, rank);

        if (!right.success)
            return ParseResult(false);

        idx = right.idx;

        node.child.push_back(left.node);
        node.child.push_back(right.node);
    }
    else
    {
        node = left.node;
    }

    return ParseResult(node, idx);
}

Parser::ParseResult Parser::ParseTerm(unsigned long idx)
{
    if (idx >= code_size)
        return FailedToParse(idx, "expected expression");

    Node node;

    switch (code[idx].type)
    {
    case TokenType::SYMBOL:
    {
        switch (code[idx].token.symbol)
        {
        case Symbol::LPAREN:
        {
            idx += 1;

            auto res = ParseExpression(idx);

            if (res.success)
            {
                idx = res.idx;
                node = res.node;
            }
            else
            {
                return ParseResult(false);
            }

            if (idx < code_size && CompSymbol(code[idx], Symbol::RPAREN))
            {
                idx += 1;
            }
            else
            {
                return FailedToParse(idx, "expected ')'");
            }
        }
        break;

        case Symbol::NOT:
        case Symbol::MINUS:
        {
            node.type = NodeType::UOPERATOR;
            node.token = code[idx];

            idx += 1;

            auto res = ParseExpression(idx);

            if (res.success)
            {
                idx = res.idx;
                node.child.push_back(res.node);
            }
            else
            {
                return ParseResult(false);
            }
        }
        break;

        default:
            return FailedToParse(idx, "expected expression");
        }
        break;

    case TokenType::CONST:
    {
        node.type = NodeType::CONST;
        node.token = code[idx];
        idx += 1;
    }
    break;

    case TokenType::OTHER:
    {
        node.token = code[idx];

        if (idx + 1 < code_size && CompSymbol(code[idx + 1], Symbol::LPAREN))
        {
            node.type = NodeType::FUNC;

            idx += 2;

            if (CompSymbol(code[idx], Symbol::RPAREN))
            {
                idx += 1;
            }
            else
            {
                bool ok = false;

                while (idx < code_size)
                {
                    auto res = ParseExpression(idx);

                    if (res.success)
                    {
                        idx = res.idx;
                        node.child.push_back(res.node);
                    }
                    else
                    {
                        return ParseResult(false);
                    }

                    if (CompSymbol(code[idx], Symbol::RPAREN))
                    {
                        idx += 1;
                        ok = true;
                        break;
                    }

                    if (CompSymbol(code[idx], Symbol::COMMMA))
                    {
                        idx += 1;
                    }
                }

                if (!ok)
                    return FailedToParse(idx, "expected ')'");
            }
        }
        else
        {
            node.type = NodeType::VAR;
            idx += 1;
        }
    }
    break;

    default:
        return FailedToParse(idx, "expected expression");
    }
    }

    return ParseResult(node, idx);
}

Parser::ParseResult Parser::ParseVariable(unsigned long idx)
{
    if (idx >= code_size)
        return FailedToParse(idx, "expected variable");

    if (code[idx].type != TokenType::OTHER)
        return FailedToParse(idx, "expected variable");

    Node node;
    node.type = NodeType::VAR;
    node.token = code[idx];

    return ParseResult(node, idx + 1);
}

void Parser::Node::Out(unsigned long long indent_size, std::vector<VM::Obj> &const_table)
{
    std::string indent(indent_size * 2, ' ');

    printf("%s", indent.c_str());

    switch (type)
    {
    case NodeType::ROOT:
        printf("%s ", STR(ROOT));
        break;
    case NodeType::BLOCK:
        printf("%s ", STR(BLOCK));
        break;
    case NodeType::DEF_FUNC:
        printf("%s ", STR(DEF_FUNC));
        break;
    case NodeType::FUNC:
        printf("%s ", STR(FUNC));
        break;
    case NodeType::VAR:
        printf("%s ", STR(VAR));
        break;
    case NodeType::CONST:
        printf("%s ", STR(CONST));
        break;
    case NodeType::ASSIGN:
        printf("%s ", STR(ASSIGN));
        break;
    case NodeType::UOPERATOR:
        printf("%s ", STR(UOPERATOR));
        break;
    case NodeType::BOPERATOR:
        printf("%s ", STR(BOPERATOR));
        break;
    case NodeType::RETURN:
        printf("%s ", STR(RETURN));
        break;
    case NodeType::REPEAT:
        printf("%s ", STR(REPEAT));
        break;
    case NodeType::FOR:
        printf("%s ", STR(FOR));
        break;
    case NodeType::WHILE:
        printf("%s ", STR(WHILE));
        break;
    case NodeType::IF:
        printf("%s ", STR(IF));
        break;
    }

    switch (type)
    {
    case NodeType::UOPERATOR:
    case NodeType::BOPERATOR:
        switch (token.token.symbol)
        {
        case Symbol::PLUS:
            puts(STR(PLUS));
            break;
        case Symbol::MINUS:
            puts(STR(MINUS));
            break;
        case Symbol::MUL:
            puts(STR(MUL));
            break;
        case Symbol::DIV:
            puts(STR(DIV));
            break;
        case Symbol::MOD:
            puts(STR(MOD));
            break;
        case Symbol::BAND:
            puts(STR(BAND));
            break;
        case Symbol::BOR:
            puts(STR(BOR));
            break;
        case Symbol::BXOR:
            puts(STR(BXOR));
            break;
        case Symbol::BNOT:
            puts(STR(BNOT));
            break;
        case Symbol::AND:
            puts(STR(AND));
            break;
        case Symbol::OR:
            puts(STR(OR));
            break;
        case Symbol::NOT:
            puts(STR(NOT));
            break;
        case Symbol::EQUAL:
            puts(STR(EQUAL));
            break;
        case Symbol::NEQUAL:
            puts(STR(NEQUAL));
            break;
        case Symbol::GRE:
            puts(STR(GRE));
            break;
        case Symbol::GREEQ:
            puts(STR(GREEQ));
            break;
        case Symbol::LESS:
            puts(STR(LESS));
            break;
        case Symbol::LESSEQ:
            puts(STR(LESSEQ));
            break;
        case Symbol::QUE:
            puts(STR(QUE));
            break;
        default:
            puts("");
            break;
        }

        break;

    case NodeType::CONST:
        const_table[token.token.val].Out();
        break;

    case NodeType::DEF_FUNC:
    case NodeType::FUNC:
    case NodeType::VAR:
        printf("%s %ld\n", token.str.c_str(), token.token.val);
        break;

    default:
        puts("");
        break;
    }

    for (auto c : child)
    {
        c.Out(indent_size + 1, const_table);
    }
}

bool Parser::GenerateIseq()
{
    iseq = std::vector<unsigned long>();

    return GenerateInst(ast);
}

bool Parser::GenerateInst(Node node)
{
    switch (node.type)
    {
    case NodeType::ROOT:

        break;

    case NodeType::BLOCK:

        break;

    case NodeType::DEF_FUNC:

        break;

    case NodeType::FUNC:

        break;

    case NodeType::VAR:

        break;

    case NodeType::CONST:

        PushInst(VM::Inst::PUSH_CONST);
        PushInst(node.token.token.val);

        break;

    case NodeType::ASSIGN:

        break;

    case NodeType::UOPERATOR:
        switch (node.token.token.symbol)
        {
        case NOT:
            GenerateInst(node.child[0]);
            PushInst(VM::Inst::NOT);
            break;

        case MINUS:
            PushInst(VM::Inst::PUSH_ZERO);
            GenerateInst(node.child[0]);
            PushInst(VM::Inst::BOPE);
            PushInst(VM::Inst::SUB);

            break;

        default:
            return false;
        }

        break;

    case NodeType::BOPERATOR:

        for (auto c : node.child)
        {
            if (!GenerateInst(c))
                return false;
        }

        PushInst(VM::Inst::BOPE);

        switch (node.token.token.symbol)
        {
        case PLUS:
            PushInst(VM::Inst::ADD);
            break;

        case Symbol::MINUS:
            PushInst(VM::Inst::SUB);
            break;

        case Symbol::MUL:
            PushInst(VM::Inst::MUL);
            break;

        case Symbol::DIV:
            PushInst(VM::Inst::DIV);
            break;

        case Symbol::MOD:
            PushInst(VM::Inst::MOD);
            break;

        case Symbol::BAND:
            //TODO:
            break;

        case Symbol::BOR:
            //TODO:
            break;

        case Symbol::BXOR:
            //TODO:
            break;

        case Symbol::BNOT:
            //TODO:
            break;

        case Symbol::AND:
            PushInst(VM::Inst::AND);
            break;

        case Symbol::OR:
            PushInst(VM::Inst::OR);
            break;

        case Symbol::NOT:
            PushInst(VM::Inst::NOT);
            break;

        case Symbol::EQUAL:
            PushInst(VM::Inst::EQUAL);
            break;

        case Symbol::NEQUAL:
            PushInst(VM::Inst::NOT_EQUAL);
            break;

        case Symbol::GRE:
            PushInst(VM::Inst::GREATER_THAN);
            break;

        case Symbol::GREEQ:
            PushInst(VM::Inst::GREATER_THAN_OR_EQUAL);
            break;

        case Symbol::LESS:
            PushInst(VM::Inst::LESS_THAN);
            break;

        case Symbol::LESSEQ:
            PushInst(VM::Inst::LESS_THAN_OR_EQUAL);
            break;

        default:

            break;
        }

        break;

    case NodeType::RETURN:

        break;

    case NodeType::REPEAT:

        break;

    case NodeType::FOR:

        break;

    case NodeType::WHILE:

        break;

    case NodeType::IF:

        break;
    }

    return false;
}
