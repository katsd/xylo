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
    code = std::vector<std::string>();

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

    return Result(true, code);
}
