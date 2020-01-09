//
// xylo.hpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#ifndef XYLO_HPP_
#define XYLO_HPP_

#include <string>

#include "parser.hpp"
#include "vm/vm.hpp"

class Xylo
{
private:
    VM eval;

public:
    Xylo(std::string code_str)
    {
        auto parser_res = Parser(code_str).Parse();

        if (parser_res.success)
            eval = VM(parser_res.iseq, parser_res.const_table);

        eval.OutIseq();

        eval.Init();
    }

    VM::Result Run(unsigned long func_id)
    {
        return eval.RunFunc(func_id);
    }
};

#endif /* XYLO_HPP_ */
