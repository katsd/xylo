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

    unsigned long index = 0;

    while (index < code_str_size)
    {
        if (index + 1 < code_str_size && code_str[index] == '/')
        {
            if (code_str[index + 1] == '/')
            {
                while (index < code_str_size && !(0 <= index - 1 && code_str[index - 1] == '\n'))
                    index += 1;
            }
            else if (code_str[index + 1] == '*')
            {
                while (index < code_str_size && !(0 <= index - 2 && code_str[index - 2] == '*' && code_str[index - 1] == '/'))
                    index += 1;
            }
        }

        if (index < code_str_size)
        {
            new_code_str += code_str[index];
            index += 1;
        }
    }

    return new_code_str;
}

Lexer::Result Lexer::Tokenize()
{
    code = std::vector<std::string>();

    code_str = RemoveComment(code_str);

    std::string current = "";

    unsigned long index = 0;

    return Result(true, code);
}
