//
// lexer.hpp
// xylo
//
// Copyright (c) 2020 Katsu Matsuda. All rights reserved.
//

#ifndef LEXER_CPP_
#define LEXER_CPP_

#include <stdio.h>
#include <string>
#include <vector>

class Lexer
{
private:
    std::string code_str;

    std::vector<std::string> code;

    std::string RemoveComment(std::string code_str);

public:
    struct Result
    {
        bool success;
        std::vector<std::string> code;

        Result(bool success, std::vector<std::string> code)
        {
            this->success = success;
            this->code = code;
        }
    };

    Result Tokenize();

    Lexer(std::string code_str)
    {
        this->code_str = code_str;
    }
};

#endif /* LEXER_CPP_ */
