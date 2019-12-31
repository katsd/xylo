//
// vm_test.cpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#include <stdio.h>
#include "../src/vm/vm.hpp"

#define MIN 1e-15

void out(bool state)
{
    if (state)
        printf("\x1b[32msuccessful\x1b[39m\n");
    else
        printf("\x1b[31mfailed\x1b[39m\n");
}

int main()
{
    printf("\n### Xylo VM Test ###\n\n");

    {
        printf("return int value : ");

        std::vector<unsigned long> iseq = {VM::START, VM::PUSH_CONST, 0, VM::END};
        std::vector<VM::Obj> const_table = {VM::Obj(128)};

        VM eval = VM(iseq, const_table);

        VM::State state = eval.Run(0);

        if (state.success && state.res.GetInt() == 128)
            out(true);
        else
            out(false);
    }

    {
        printf("return float value : ");

        std::vector<unsigned long> iseq = {VM::START, VM::PUSH_CONST, 0, VM::END};
        std::vector<VM::Obj> const_table = {VM::Obj(3.141592)};

        VM eval = VM(iseq, const_table);

        VM::State state = eval.Run(0);

        if (state.success && state.res.GetFloat() == 3.141592)
            out(true);
        else
            out(false);
    }

    {
        printf("int + int : ");

        std::vector<unsigned long> iseq = {VM::START, VM::PUSH_CONST, 0, VM::PUSH_CONST, 1, VM::BOPE, VM::ADD, VM::END};
        std::vector<VM::Obj> const_table = {VM::Obj(128), VM::Obj(256)};

        VM eval = VM(iseq, const_table);

        VM::State state = eval.Run(0);

        if (state.success && state.res.GetInt() == 384)
            out(true);
        else
            out(false);
    }

    {
        printf("float + float : ");

        std::vector<unsigned long> iseq = {VM::START, VM::PUSH_CONST, 0, VM::PUSH_CONST, 1, VM::BOPE, VM::ADD, VM::END};
        std::vector<VM::Obj> const_table = {VM::Obj(3.14), VM::Obj(2.17)};

        VM eval = VM(iseq, const_table);

        VM::State state = eval.Run(0);

        if (state.success && (state.res.GetFloat() - 5.31) < MIN)
            out(true);
        else
            out(false);
    }

    return 0;
}
