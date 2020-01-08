//
// vm_test.cpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#include <stdio.h>
#include "../src/vm/vm.hpp"

#define MIN 1e-15

void out(bool result)
{
    if (result)
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

        VM::Result result = eval.Run(0);

        if (result.success && result.res.GetInt() == 128)
            out(true);
        else
            out(false);
    }

    {
        printf("return float value : ");

        std::vector<unsigned long> iseq = {VM::START, VM::PUSH_CONST, 0, VM::END};
        std::vector<VM::Obj> const_table = {VM::Obj(3.141592)};

        VM eval = VM(iseq, const_table);

        VM::Result result = eval.Run(0);

        if (result.success && result.res.GetFloat() == 3.141592)
            out(true);
        else
            out(false);
    }

    {
        printf("int + int : ");

        std::vector<unsigned long> iseq = {VM::START, VM::PUSH_CONST, 0, VM::PUSH_CONST, 1, VM::BOPE, VM::ADD, VM::END};
        std::vector<VM::Obj> const_table = {VM::Obj(128), VM::Obj(256)};

        VM eval = VM(iseq, const_table);

        VM::Result result = eval.Run(0);

        if (result.success && result.res.GetInt() == 384)
            out(true);
        else
            out(false);
    }

    {
        printf("float + float : ");

        std::vector<unsigned long> iseq = {VM::START, VM::PUSH_CONST, 0, VM::PUSH_CONST, 1, VM::BOPE, VM::ADD, VM::END};
        std::vector<VM::Obj> const_table = {VM::Obj(3.14), VM::Obj(2.17)};

        VM eval = VM(iseq, const_table);

        VM::Result result = eval.Run(0);

        if (result.success && (result.res.GetFloat() - 5.31) < MIN)
            out(true);
        else
            out(false);
    }

    {
        printf("loop : ");

        std::vector<unsigned long> iseq = {VM::START,
                                           VM::PUSH_CONST,
                                           0,
                                           VM::SET_OBJ,
                                           0,
                                           VM::PUSH_OBJ,
                                           0,
                                           VM::PUSH_CONST,
                                           1,
                                           VM::BOPE,
                                           VM::ADD,
                                           VM::SET_OBJ,
                                           0,
                                           VM::PUSH_OBJ,
                                           0,
                                           VM::PUSH_CONST,
                                           2,
                                           VM::BOPE,
                                           VM::LESS_THAN,
                                           VM::JUMP_IF,
                                           5,
                                           VM::PUSH_OBJ,
                                           0,
                                           VM::END};

        std::vector<VM::Obj> const_table = {VM::Obj(0), VM::Obj(1), VM::Obj(100000)};

        VM eval = VM(iseq, const_table);

        VM::Result result = eval.Run(0);

        if (result.success && result.res.GetInt() == 100000)
            out(true);
        else
            out(false);
    }

    {
        printf("fibonacci : ");

        std::vector<unsigned long> iseq = {VM::START,
                                           VM::PUSH_CONST,
                                           0,
                                           VM::SET_OBJ,
                                           3,
                                           VM::PUSH_CONST,
                                           0,
                                           VM::SET_OBJ,
                                           0,
                                           VM::PUSH_CONST,
                                           1,
                                           VM::SET_OBJ,
                                           1,
                                           /*
                                           VM::PUSH_OBJ,
                                           1,
                                           VM::OUT,
                                           */
                                           VM::PUSH_OBJ,
                                           1,
                                           VM::SET_OBJ,
                                           2,
                                           VM::PUSH_OBJ,
                                           0,
                                           VM::PUSH_OBJ,
                                           1,
                                           VM::BOPE,
                                           VM::ADD,
                                           VM::SET_OBJ,
                                           1,
                                           VM::PUSH_OBJ,
                                           2,
                                           VM::SET_OBJ,
                                           0,
                                           VM::PUSH_OBJ,
                                           3,
                                           VM::PUSH_CONST,
                                           1,
                                           VM::BOPE,
                                           VM::ADD,
                                           VM::SET_OBJ,
                                           3,
                                           VM::PUSH_OBJ,
                                           3,
                                           VM::PUSH_CONST,
                                           2,
                                           VM::BOPE,
                                           VM::LESS_THAN,
                                           VM::JUMP_IF,
                                           13,
                                           VM::PUSH_OBJ,
                                           1,
                                           VM::END};

        std::vector<VM::Obj> const_table = {VM::Obj(0), VM::Obj(1), VM::Obj(41)};

        VM eval = VM(iseq, const_table);

        VM::Result result = eval.Run(0);

        if (result.success && result.res.GetInt() == 267914296)
            out(true);
        else
            out(false);
    }

    return 0;
}
