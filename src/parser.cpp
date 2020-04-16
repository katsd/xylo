//
// parser.cpp
// xylo
//
// Copyright (c) 2020 Katsu Matsuda. All rights reserved.
//

#include "parser.hpp"
#include "native.hpp"

#define STR(var) #var

Parser::Result Parser::Parse()
{
    code_str += Native::StandardLibraryCode();

    puts(code_str.c_str());

    auto lexer_res = Lexer(code_str).Tokenize();

    if (!lexer_res.success)
    {
        printf("failed to tokenize\n");
        return Result(false, std::vector<unsigned long>(), std::vector<VM::Obj>());
    }

    code = lexer_res.code;

    const_table = lexer_res.const_table;

    if (!GenerateAST())
    {
        printf("failed to parse\n");
        return Result(false, std::vector<unsigned long>(), std::vector<VM::Obj>());
    }

    if (!GenerateIseq())
    {
        printf("failed to generate iseq\n");
        return Result(false, std::vector<unsigned long>(), std::vector<VM::Obj>());
    }

    /*
    for (auto i : iseq)
        printf("%ld\n", i);
    */

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

    //ast.Out(0, const_table);

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

            idx += 1;

            if (idx < code.size() && CompReserved(code[idx], Reserved::NATIVE))
            {
                Node native;
                native.type = NodeType::NATIVE;
                idx += 1;

                auto func_id = ParseExpression(idx);
                idx = func_id.idx;

                if (func_id.success)
                {
                    native.child.push_back(func_id.node);
                }
                else
                {
                    return ParseResult(false);
                }

                node.child.push_back(native);
            }

            auto res = ParseVariable(idx);

            if (res.success)
            {
                idx = res.idx;
                node.child.push_back(res.node);
            }
            else
            {
                return FailedToParse(idx, "expected func name");
            }

            if (idx < code_size && CompSymbol(code[idx], Symbol::LPAREN))
            {
                idx += 1;

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

                    Node return_node = Node(NodeType::RETURN, Token());
                    return_node.child.push_back(Node(CONST, Token(TokenType::CONST, TokenVal(0), "")));
                    node.child[node.child.size() - 1].child.push_back(return_node);
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
                    bool in_bracket = idx < code_size && CompSymbol(code[idx], Symbol::LBRACKET);

                    auto res = ParseStatement(idx);

                    if (res.success)
                    {
                        idx = res.idx;
                        if (in_bracket)
                        {
                            node.child.push_back(res.node);
                        }
                        else
                        {
                            node.child.push_back(Node::BlockNode());
                            node.child[2].child.push_back(res.node);
                        }
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
                bool in_bracket = idx < code_size && CompSymbol(code[idx], Symbol::LBRACKET);

                auto res = ParseStatement(idx);

                if (res.success)
                {
                    idx = res.idx;
                    if (in_bracket)
                    {
                        node.child.push_back(res.node);
                    }
                    else
                    {
                        node.child.push_back(Node::BlockNode());
                        node.child[1].child.push_back(res.node);
                    }
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
                bool in_bracket = idx < code_size && CompSymbol(code[idx], Symbol::LBRACKET);

                auto res = ParseStatement(idx);

                if (res.success)
                {
                    idx = res.idx;
                    if (in_bracket)
                    {
                        node.child.push_back(res.node);
                    }
                    else
                    {
                        node.child.push_back(Node::BlockNode());
                        node.child[1].child.push_back(res.node);
                    }
                }
                else
                {
                    return ParseResult(false);
                }
            }

            if (idx < code_size && CompReserved(code[idx], Reserved::ELSE))
            {
                idx += 1;

                bool in_bracket = idx < code_size && CompSymbol(code[idx], Symbol::LBRACKET);

                auto res = ParseStatement(idx);

                if (res.success)
                {
                    idx = res.idx;
                    if (in_bracket)
                    {
                        node.child.push_back(res.node);
                    }
                    else
                    {
                        node.child.push_back(Node::BlockNode());
                        node.child[2].child.push_back(res.node);
                    }
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

    auto exps = std::vector<Node>();
    auto opes = std::vector<Token>();

    auto exp = ParseExpression(idx, rank + 1);

    if (!exp.success)
        return ParseResult(false);

    exps.push_back(exp.node);

    idx = exp.idx;

    while (idx < code_size && code[idx].type == TokenType::SYMBOL
           && operator_rank[rank].count(code[idx].token.symbol) > 0)
    {
        opes.push_back(code[idx]);
        idx += 1;

        auto exp = ParseExpression(idx, rank + 1);

        if (!exp.success)
            return ParseResult(false);

        exps.push_back(exp.node);

        idx = exp.idx;
    }

    auto node = exps[0];

    for (int i = 1; i < exps.size(); i++)
    {
        Node newNode;
        newNode.child.push_back(node);
        newNode.child.push_back(exps[i]);
        newNode.type = NodeType::BOPERATOR;
        newNode.token = opes[i - 1];

        node = newNode;
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
        case Symbol::BNOT:
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

void Parser::Node::Out(unsigned long long indent_size, std::vector<VM::Obj>& const_table)
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
    case NodeType::NATIVE:
        printf("%s ", STR(NATIVE));
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
    block_cnt = 1;

    block_is_alive = std::map<unsigned long, bool>();
    block_is_alive[block_cnt] = true;

    var_cnt = 0;

    global_var_cnt = 0;

    var_address = std::map<unsigned long, unsigned long>();

    var_block_id = std::map<unsigned long, unsigned long>();

    func_data = std::set<Func>();

    func_info = std::map<Func, FuncInfo>();

    unassigned_func_start_idx = std::map<unsigned long, Func>();

    iseq = std::vector<unsigned long>();

    global_block_id = block_cnt;

    block_is_alive[global_block_id] = true;

    for (Node n : ast.child)
    {
        if (n.type == NodeType::DEF_FUNC)
        {
            if (!DefineFunc(n))
                return false;
        }
    }

    PushInst(VM::Inst::START);

    for (Node n : ast.child)
    {
        if (n.type == NodeType::DEF_FUNC)
            continue;

        if (!GenerateInst(n, ast, global_block_id))
            return false;
    }

    PushInst(VM::Inst::END);

    for (Node n : ast.child)
    {
        if (n.type != NodeType::DEF_FUNC)
            continue;

        if (!GenerateInst(n, ast, global_block_id))
            return false;
    }

    for (auto i : unassigned_func_start_idx)
    {
        iseq[i.first] = func_info[i.second].start_idx;
    }

    return true;
}

bool Parser::DefineFunc(Node node)
{
    bool is_native_func = false;
    Node native_func_id;

    if (!node.child.empty() && node.child[0].type == NodeType::NATIVE)
    {
        if (node.child.empty())
        {
            printf("native func id is not given\n");
            return false;
        }

        is_native_func = true;
        native_func_id = node.child[0].child[0];
    }

    unsigned long func_name = node.child[0 + is_native_func].token.token.val;

    unsigned long arg_num = node.child.size() - 2 - is_native_func;

    Func func = Func(func_name, arg_num);

    if (func_data.count(func) > 0)
    {
        printf("function %s is already declared\n", node.child[0].token.str.c_str());
        return false;
    }

    func_data.insert(func);

    func_info[func].is_native_func = is_native_func;

    //TODO: eval native_func_id
    func_info[func].native_func_id = native_func_id;

    return true;
}

bool Parser::GenerateInst(Node node, const Node& par, unsigned long block_id)
{
    switch (node.type)
    {
    case NodeType::ROOT:
        break;

    case NodeType::BLOCK:
    {
        unsigned long current_var_cnt = var_cnt;

        block_cnt += 1;

        block_is_alive[block_cnt] = true;

        for (auto c : node.child)
            GenerateInst(c, node, block_cnt);

        block_is_alive[block_cnt] = false;

        var_cnt = current_var_cnt;
    }

        break;

    case NodeType::DEF_FUNC:
    {
        if (par.type != NodeType::ROOT)
            return false;

        bool is_native_func = false;

        if (!node.child.empty() && node.child[0].type == NodeType::NATIVE)
        {
            if (node.child.empty())
            {
                printf("native func id is not given\n");
                return false;
            }

            is_native_func = true;
        }

        PushInst(VM::Inst::START);

        for (int i = node.child.size() - 2; i >= 1 + is_native_func; i--)
        {
            if (node.child[i].type != NodeType::VAR)
                return false;

            unsigned long var_name = node.child[i].token.token.val;

            if (IsVarDeclared(var_name))
            {
                printf("variable %s is already declared\n", node.child[i].token.str.c_str());
                return false;
            }

            unsigned long address = DeclareVar(var_name, block_id + 1);

            PushInst(VM::Inst::SET_OBJ);
            PushInst(address);
        }

        GenerateInst(node.child[node.child.size() - 1], node, block_id);

        PushInst(VM::Inst::END);
    }
        break;

    case NodeType::FUNC:
    {
        if (node.token.str == "print")
        {
            GenerateInst(node.child[0], node, block_id);
            PushInst(VM::Inst::OUT);

            break;
        }

        auto func = Func(node.token.token.val, node.child.size());

        if (func_info[func].is_native_func)
        {
            for (auto i = (long)node.child.size() - 1; i >= 0; i--)
                GenerateInst(node.child[i], node, block_id);

            PushInst(VM::Inst::PUSH);
            PushInst(func.arg_num);
            GenerateInst(func_info[func].native_func_id, Node(NodeType::BLOCK, Token()), block_id);
            PushInst(VM::Inst::CALL_NATIVE);

            break;
        }

        PushInst(VM::Inst::ICR_FUNC_LEVEL);

        PushInst(VM::Inst::PUSH);
        PushInst(VM::Inst::ERROR);

        unsigned long pos = iseq.size() - 1;

        PushInst(VM::Inst::PUSH_OBJ_MIN_IDX);

        PushInst(VM::Inst::PUSH_START);

        for (int i = 0; i < node.child.size(); i++)
        {
            GenerateInst(node.child[i], node, block_id);
        }

        PushInst(VM::Inst::ADD_OBJ_MIN_IDX);
        PushInst(var_cnt + 1);

        PushInst(VM::Inst::JUMP);
        PushInst(VM::Inst::ERROR);
        unassigned_func_start_idx[iseq.size() - 1] = func;

        iseq[pos] = iseq.size();

        PushInst(VM::Inst::SET_OBJ_MIN_IDX);
    }

        break;

    case NodeType::VAR:
        if (!PushVar(node.token.token.val, node.token.str))
            return false;

        break;

    case NodeType::CONST:

        PushInst(VM::Inst::PUSH_CONST);
        PushInst(node.token.token.val);

        break;

    case NodeType::ASSIGN:
    {
        unsigned long var_name = node.child[0].token.token.val;

        if (!IsVarDeclared(var_name))
        {
            if (block_id == global_block_id)
                DeclareGlobalVar(var_name);
            else
                DeclareVar(var_name, block_id);
        }

        unsigned long address = var_address[var_name];

        GenerateInst(node.child[1], node, block_id);

        if (var_block_id[var_name] == global_block_id)
            PushInst(VM::Inst::SET_GLOBAL_OBJ);
        else
            PushInst(VM::Inst::SET_OBJ);
        PushInst(address);
    }

        break;

    case NodeType::UOPERATOR:
        switch (node.token.token.symbol)
        {
        case NOT:
            GenerateInst(node.child[0], node, block_id);
            PushInst(VM::Inst::NOT);
            break;

        case BNOT:
            GenerateInst(node.child[0], node, block_id);
            PushInst(VM::Inst::BNOT);

            break;

        case MINUS:
            PushInst(VM::Inst::PUSH_ZERO);
            GenerateInst(node.child[0], node, block_id);
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
            if (!GenerateInst(c, node, block_id))
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
            PushInst(VM::Inst::BAND);
            break;

        case Symbol::BOR:
            PushInst(VM::Inst::BOR);
            break;

        case Symbol::BXOR:
            PushInst(VM::Inst::BXOR);
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
    {
        unsigned long return_var_address = GetTmpVar();
        unsigned long obj_min_idx_var_address = GetTmpVar();
        unsigned long back_pc_var_address = GetTmpVar();

        GenerateInst(node.child[0], node, block_id);

        PushInst(VM::Inst::DCR_FUNC_LEVEL);

        PushInst(VM::Inst::SET_OBJ);
        PushInst(return_var_address);

        PushInst(VM::Inst::POP_TO_START);

        PushInst(VM::Inst::SET_OBJ);
        PushInst(obj_min_idx_var_address);

        PushInst(VM::Inst::SET_OBJ);
        PushInst(back_pc_var_address);

        PushInst(VM::Inst::PUSH_OBJ);
        PushInst(return_var_address);

        PushInst(VM::Inst::PUSH_OBJ);
        PushInst(obj_min_idx_var_address);

        PushInst(VM::Inst::PUSH_OBJ);
        PushInst(back_pc_var_address);

        PushInst(VM::Inst::JUMP2);

        ReturnTmpVar();
        ReturnTmpVar();
        ReturnTmpVar();
    }

        break;

    case NodeType::REPEAT:
    {
        unsigned long time_address = GetTmpVar();

        GenerateInst(node.child[0], node, block_id);
        PushInst(VM::Inst::SET_OBJ);
        PushInst(time_address);

        unsigned long return_pos = iseq.size();

        PushInst(VM::Inst::PUSH_OBJ);
        PushInst(time_address);
        PushInst(VM::Inst::PUSH_ZERO);
        PushInst(VM::Inst::BOPE);
        PushInst(VM::Inst::LESS_THAN_OR_EQUAL);
        PushInst(VM::Inst::JUMP_IF);
        PushInst(VM::Inst::ERROR);

        unsigned long pos = iseq.size() - 1;

        GenerateInst(node.child[1], node, block_id);

        PushInst(VM::Inst::PUSH_OBJ);
        PushInst(time_address);
        PushInst(VM::Inst::PUSH_ONE);
        PushInst(VM::Inst::BOPE);
        PushInst(VM::Inst::SUB);
        PushInst(VM::Inst::SET_OBJ);
        PushInst(time_address);
        PushInst(VM::Inst::JUMP);
        PushInst(return_pos);

        iseq[pos] = iseq.size();

        ReturnTmpVar();
    }

        break;

    case NodeType::FOR:
    {
        unsigned long cnt_var_name = node.child[0].token.token.val;

        if (IsVarDeclared(cnt_var_name))
        {
            printf("variable %s is already declared", node.child[0].token.str.c_str());
            return false;
        }

        unsigned long cnt_var_address = DeclareVar(cnt_var_name, block_id + 1);

        unsigned long max_var_address = GetTmpVar();

        GenerateInst(node.child[1], node, block_id);
        PushInst(VM::Inst::SET_OBJ);
        PushInst(max_var_address);

        PushInst(VM::Inst::PUSH_ZERO);
        PushInst(VM::Inst::SET_OBJ);
        PushInst(cnt_var_address);

        unsigned long return_pos = iseq.size();

        PushInst(VM::Inst::PUSH_OBJ);
        PushInst(cnt_var_address);
        PushInst(VM::Inst::PUSH_OBJ);
        PushInst(max_var_address);
        PushInst(VM::Inst::BOPE);
        PushInst(VM::Inst::GREATER_THAN_OR_EQUAL);
        PushInst(VM::Inst::JUMP_IF);
        PushInst(VM::Inst::ERROR);

        unsigned long pos = iseq.size() - 1;

        GenerateInst(node.child[2], node, block_id);

        PushInst(VM::Inst::PUSH_OBJ);
        PushInst(cnt_var_address);
        PushInst(VM::Inst::PUSH_ONE);
        PushInst(VM::Inst::BOPE);
        PushInst(VM::Inst::ADD);
        PushInst(VM::Inst::SET_OBJ);
        PushInst(cnt_var_address);
        PushInst(VM::Inst::JUMP);
        PushInst(return_pos);

        iseq[pos] = iseq.size();

        ReturnTmpVar();
    }

        break;

    case NodeType::WHILE:
    {
        GenerateInst(node.child[0], node, block_id);
        PushInst(VM::Inst::NOT);
        PushInst(VM::Inst::JUMP_IF);
        PushInst(VM::Inst::ERROR);

        unsigned long pos = iseq.size() - 1;

        unsigned long return_pos = iseq.size();

        GenerateInst(node.child[1], node, block_id);

        GenerateInst(node.child[0], node, block_id);
        PushInst(VM::Inst::JUMP_IF);
        PushInst(return_pos);

        iseq[pos] = iseq.size();
    }

        break;

    case NodeType::IF:
    {
        bool has_else = node.child.size() == 3;

        GenerateInst(node.child[0], node, block_id);

        if (has_else)
        {
            unsigned long tmp_var_address = GetTmpVar();

            PushInst(VM::Inst::PUSH_TOP);
            PushInst(VM::Inst::SET_OBJ);
            PushInst(tmp_var_address);

            PushInst(VM::Inst::NOT);
            PushInst(VM::Inst::JUMP_IF);
            PushInst(VM::Inst::ERROR);

            unsigned long pos1 = iseq.size() - 1;

            GenerateInst(node.child[1], node, block_id);

            iseq[pos1] = iseq.size();

            PushInst(VM::Inst::JUMP);
            PushInst(VM::Inst::ERROR);

            unsigned long pos2 = iseq.size() - 1;

            GenerateInst(node.child[2], node, block_id);

            iseq[pos2] = iseq.size();

            ReturnTmpVar();
        }
        else
        {

            PushInst(VM::Inst::NOT);
            PushInst(VM::Inst::JUMP_IF);
            PushInst(VM::Inst::ERROR);

            unsigned long pos = iseq.size() - 1;

            GenerateInst(node.child[1], node, block_id);

            iseq[pos] = iseq.size();
        }
    }

        break;

    default:
        break;
    }

    return true;
}

bool Parser::PushVar(const unsigned long var_name, const std::string& var_name_str)
{
    if (!IsVarDeclared(var_name))
    {
        printf("variable %s is not declared.\n", var_name_str.c_str());
        return false;
    }

    unsigned long address = var_address[var_name];

    if (var_block_id[address] == global_block_id)
        PushInst(VM::Inst::PUSH_GLOBAL_OBJ);
    else
        PushInst(VM::Inst::PUSH_OBJ);
    PushInst(address);

    return true;
}

bool Parser::IsVarDeclared(const unsigned long var_name)
{
    unsigned long address = var_address[var_name];
    if (address == 0)
        return false;

    if (!block_is_alive[var_block_id[address]])
        return false;

    return true;
}
