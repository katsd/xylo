//
// lexer.cpp
// xylo
//
// Copyright (c) 2020 Katsu Matsuda. All rights reserved.
//

#include "lexer.hpp"

std::string Lexer::RemoveComment(std::string code_str)
{
    std::string new_code_str = "";

    unsigned long code_str_size = code_str.size();

    unsigned long idx = 0;

    while (idx < code_str_size)
    {
        if (idx + 1 < code_str_size && code_str[idx] == '/')
        {
            if (code_str[idx + 1] == '/')
            {
                idx += 2;
                while (idx < code_str_size && !(0 <= idx - 1 && code_str[idx - 1] == '\n'))
                    idx += 1;
            }
            else if (code_str[idx + 1] == '*')
            {
                idx += 2;
                while (idx < code_str_size && !(0 <= idx - 2 && code_str[idx - 2] == '*' && code_str[idx - 1] == '/'))
                    idx += 1;
            }
        }

        if (idx < code_str_size)
        {
            new_code_str += code_str[idx];
            idx += 1;
        }
    }

    return new_code_str;
}

Lexer::Result Lexer::Tokenize()
{
    success = true;

    code = std::vector<Token>();

    const_table = {VM::Obj()};

    const_num = 0;

    const_map = std::map<std::string, unsigned long>();

    other_token_num = 0;

    other_token_map = std::map<std::string, unsigned long>();

    code_str = RemoveComment(code_str);

    unsigned long code_str_size = code_str.size();

    unsigned long idx = 0;

    std::string current = "";

    while (idx < code_str_size)
    {
        char ch = code_str[idx];

        switch (ch)
        {
        case ' ':
        case '\n':
        case '\t':
            PushToken(current);
            idx += 1;

            break;

        case '<':
        case '>':
        case '=':
            if (idx + 1 < code_str_size && code_str[idx + 1] == '=')
            {
                PushToken(current);
                current = {ch, '='};
                PushToken(current);

                idx += 2;

                break;
            }

        case '&':
        case '|':
            if (idx + 1 < code_str_size && code_str[idx + 1] == ch)
            {
                PushToken(current);
                current = {ch, ch};
                PushToken(current);

                idx += 2;

                break;
            }

        case '.':
            if (0 <= idx - 1 && idx + 1 < code_str_size)
            {
                int left = code_str[idx - 1] - '0';
                int right = code_str[idx + 1] - '0';

                if (0 <= left && left <= 9 && 0 <= right && right <= 9)
                {
                    current += ch;
                    idx += 1;
                    break;
                }
            }

        case '+':
        case '-':
        case '*':
        case '/':
        case '(':
        case ')':
        case '{':
        case '}':
        case ',':
        case '!':
        case '~':
            PushToken(current);
            current = {ch};
            PushToken(current);
            idx += 1;

            break;

        default:
            current += ch;
            idx += 1;

            break;
        }
    }

    PushToken(current);

    return Result(true, code, const_table);
}

void Lexer::PushToken(std::string &token)
{
    if (token.size() <= 0)
    {
        success = false;
        return;
    }

    int first = token[0] - '0';
    int last = token[token.size() - 1] - '0';

    if (0 <= first && first <= 9 && 0 <= last && last <= 9)
    {
        bool is_number = true;

        int dot_cnt = 0;

        for (int i = 1; i < token.size() - 1; i++)
        {
            if (token[i] == '.')
            {
                dot_cnt += 1;
                continue;
            }

            int num = token[i] - '0';

            if (!(0 <= num && num <= 9))
            {
                is_number = false;
                break;
            }
        }

        if (is_number && dot_cnt <= 1)
        {
            bool is_int = dot_cnt = 0;

            unsigned long const_id;

            if (const_map[token] == 0)
            {
                const_num += 1;
                const_map[token] = const_num;

                const_id = const_num;

                if (is_int)
                    const_table.push_back(VM::Obj(Str2Int(token)));
                else
                    const_table.push_back(VM::Obj(Str2Float(token)));
            }
            else
            {
                const_id = const_map[token];
            }

            if (is_int)
                code.push_back(Token(TokenType::CONST, TokenVal(const_id), token));
            else
                code.push_back(Token(TokenType::CONST, TokenVal(const_id), token));

            return;
        }
    }

    unsigned long token_id;

    if (other_token_map[token] == 0)
    {
        other_token_num += 1;
        other_token_map[token] = other_token_num;

        token_id = other_token_num;
    }
    else
    {
        token_id = other_token_map[token];
    }

    code.push_back(Token(TokenType::OTHER, TokenVal(token_id), token));
}

long Lexer::Str2Int(std::string str)
{
    long res = 0;

    long d = 1;

    for (int i = str.size() - 1; i >= 0; i--)
    {
        res += d * (str[i] - '0');
        d *= 10;
    }

    return res;
}

double Lexer::Str2Float(std::string str)
{
    unsigned long dot_idx = str.size() - 1;

    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == '.')
        {
            dot_idx = i;
            break;
        }
    }

    double res = 0;
    double d = 1;

    for (int i = dot_idx - 1; i >= 0; i--)
    {
        res += d * (str[i] - '0');
        d *= 10;
    }

    d = 0.1;

    for (int i = dot_idx + 1; i < str.size(); i++)
    {
        res += d * (str[i] - '0');
        d *= 0.1;
    }

    return res;
}
