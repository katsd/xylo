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
                return ParseResult(false);
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
        node.type = NodeType::Statement;
        node.token = code[idx];

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
    return ParseExp(idx, 0);
}

Parser::ParseResult Parser::ParseExp(unsigned long idx, unsigned long rank)
{
    if (idx >= code_size)
        return FailedToParse(idx, "expected expression");

    if (rank >= operator_rank.size())
        return ParseTerm(idx);

    Node node;

    auto left = ParseExp(idx, rank + 1);

    if (!left.success)
        return ParseResult(false);

    idx = left.idx;

    if (idx < code_size && code[idx].type == TokenType::SYMBOL && operator_rank[rank].count(code[idx].token.symbol) > 0)
    {
        node.type = NodeType::Expression;
        node.token = code[idx];

        idx += 1;

        auto right = ParseExp(idx, rank);

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
            node.type = NodeType::Expression;
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
        node.type = NodeType::Expression;
        node.token = code[idx];
        idx += 1;
    }
    break;

    case TokenType::OTHER:
    {
        node.type = NodeType::Expression;
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
        }
        else
        {
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
    node.type = NodeType::Expression;
    node.token = code[idx];

    return ParseResult(node, idx + 1);
}

bool Parser::GenerateIseq()
{
    return false;
}

void Parser::Node::Out(unsigned long long indent_size, std::vector<VM::Obj> &const_table)
{
    std::string indent(indent_size * 2, ' ');

    printf("%s", indent.c_str());

    switch (type)
    {
    case Expression:
        printf("%s ", (indent + "Exp").c_str());
        break;
    case Statement:
        printf("%s ", (indent + "Sta").c_str());
        break;
    case Root:
        printf("%s ", (indent + "Root").c_str());
        break;
    default:
        break;
    }

    switch (token.type)
    {
    case TokenType::RESERVED:
        switch (token.token.reserved)
        {
        case FUNC:
            puts(STR(FUNC));
            break;
        case RETURN:
            puts(STR(RETURN));
            break;
        case REPEAT:
            puts(STR(REPEAT));
            break;
        case FOR:
            puts(STR(FOR));
            break;
        case WHILE:
            puts(STR(WHILE));
            break;
        case IF:
            puts(STR(IF));
            break;
        case ELSE:
            puts(STR(ELSE));
            break;
        default:
            break;
        }
        break;

    case TokenType::SYMBOL:
        switch (token.token.symbol)
        {
        case PLUS:
            puts(STR(PLUS));
            break;
        case MINUS:
            puts(STR(MINUS));
            break;
        case MUL:
            puts(STR(MUL));
            break;
        case DIV:
            puts(STR(DIV));
            break;
        case MOD:
            puts(STR(MOD));
            break;
        case BAND:
            puts(STR(BAND));
            break;
        case BOR:
            puts(STR(BOR));
            break;
        case BXOR:
            puts(STR(BXOR));
            break;
        case BNOT:
            puts(STR(BNOT));
            break;
        case AND:
            puts(STR(AND));
            break;
        case OR:
            puts(STR(OR));
            break;
        case NOT:
            puts(STR(NOT));
            break;
        case EQUAL:
            puts(STR(EQUAL));
            break;
        case NEQUAL:
            puts(STR(NEQUAL));
            break;
        case GRE:
            puts(STR(GRE));
            break;
        case GREEQ:
            puts(STR(GREEQ));
            break;
        case LESS:
            puts(STR(LESS));
            break;
        case LESSEQ:
            puts(STR(LESSEQ));
            break;
        case ASSIGN:
            puts(STR(ASSIGN));
            break;
        case QUE:
            puts(STR(QUE));
            break;
        case COMMMA:
            puts(STR(COMMMA));
            break;
        case DOT:
            puts(STR(DOT));
            break;
        case LPAREN:
            puts(STR(LPAREN));
            break;
        case RPAREN:
            puts(STR(RPAREN));
            break;
        case LBRACKET:
            puts(STR(LBRACKET));
            break;
        case RBRACKET:
            puts(STR(RBRACKET));
            break;
        default:
            break;
        }

        break;

    case TokenType::CONST:
        printf("Const ");
        const_table[token.token.val].Out();
        break;

    case TokenType::OTHER:
        printf("Other %ld %s\n", token.token.val, token.str.c_str());
        break;
    }

    for (auto c : child)
    {
        c.Out(indent_size + 1, const_table);
    }
}