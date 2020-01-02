//
// parser.cpp
// xylo
//
// Copyright (c) 2020 Katsu Matsuda. All rights reserved.
//

#include "parser.hpp"

Parser::Result Parser::Parse()
{
    GenerateAST();

    GenerateIseq();

    return Result(true, iseq, const_table);
}

bool Parser::GenerateAST()
{
    ast = Node();

    unsigned long idx = 0;

    while (idx < 0)
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

    return true;
}

Parser::ParseResult Parser::ParseStatement(unsigned long idx)
{
    Node node;

    unsigned long code_size = code.size();

    if (idx >= code_size)
        return FailedToParse();

    switch (code[idx].type)
    {
    case TokenType::RESERVED:
    {
        switch (code[idx].token.reserved)
        {
        case Reserved::FUNC:
        {
            node.type = NodeType::Statement;
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
                            return FailedToParse();
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
                        return FailedToParse();
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
                        return FailedToParse();
                    }
                }
                else
                {
                    return FailedToParse();
                }
            }
        }
        break;

        case Reserved::RETURN:
        {
            node.type = NodeType::Statement;
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
                return FailedToParse();
            }
        }
        break;

        case Reserved::FOR:
        {
            node.type = NodeType::Statement;
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
                        return FailedToParse();
                    }
                }

                if (!(idx < code_size && CompSymbol(code[idx], Symbol::COMMMA)))
                    return FailedToParse();

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
                        return FailedToParse();
                    }
                }

                if (!(idx < code_size && CompSymbol(code[idx], Symbol::RPAREN)))
                    return FailedToParse();

                {
                    auto res = ParseStatement(idx);

                    if (res.success)
                    {
                        idx = res.idx;
                        node.child.push_back(res.node);
                    }
                    else
                    {
                        return FailedToParse();
                    }
                }
            }
            else
            {
                return FailedToParse();
            }
        }
        break;

        case Reserved::REPEAT:
        case Reserved::WHILE:
        {
            node.type = NodeType::Statement;
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
                    return FailedToParse();
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
                    return FailedToParse();
                }
            }
        }
        break;

        case Reserved::IF:
        {
            node.type = NodeType::Statement;
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
                    return FailedToParse();
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
                    return FailedToParse();
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
                    return FailedToParse();
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
        bool ok = false;

        if (code[idx].token.symbol == Symbol::LBRACKET)
        {
            idx += 1;

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
                    return FailedToParse();
                }

                if (idx < code_size && CompSymbol(code[idx], Symbol::RBRACKET))
                {
                    ok = true;
                    break;
                }
            }
        }

        if (!ok)
            return FailedToParse();
    }
    break;

    case TokenType::CONST:
    {
        return FailedToParse();
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
                node.type = NodeType::Expression;
                node.token = code[idx + 1];

                {
                    auto res = ParseVariable(idx);

                    if (res.success)
                    {
                        node.child.push_back(res.node);
                    }
                    else
                    {
                        return FailedToParse();
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
                        return FailedToParse();
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
                    return FailedToParse();
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
            return FailedToParse();
        }
    }
    break;

    default:
        return FailedToParse();
    }

    return ParseResult(true, node, idx);
}

Parser::ParseResult Parser::ParseExpression(unsigned long idx)
{
    unsigned long code_size = code.size();

    if (idx >= code_size)
        return FailedToParse();
}

Parser::ParseResult Parser::ParseVariable(unsigned long idx)
{
    unsigned long code_size = code.size();

    if (idx >= code_size)
        return FailedToParse();

    if (code[idx].type != TokenType::OTHER)
        return FailedToParse();

    Node node;
    node.type = NodeType::Expression;
    node.token = code[idx];

    return ParseResult(true, node, idx + 1);
}

bool Parser::GenerateIseq()
{
    return false;
}