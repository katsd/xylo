//
// vm.cpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#include "vm.hpp"

constexpr unsigned long obj_table_size = 1024 * 128;

constexpr unsigned long stack_size = 1024 * 128;

void VM::Run()
{
    Run(0);
}

void VM::Run(unsigned long startIndex)
{
    std::unique_ptr<Obj[]> obj_table(new Obj[obj_table_size]);

    std::unique_ptr<Obj[]> stack(new Obj[stack_size]);

    unsigned long pc = startIndex;

    unsigned long sc = 0;

    if (iseq[pc] != START)
        return;

    pc += 1;

    while (true)
    {
        unsigned long inst = iseq[pc];

        if (inst == END)
            break;

        if (inst == START)
            break;

        switch (inst)
        {
        case PUSH:
            stack[sc] = Obj((long)iseq[pc + 1]);
            sc += 1;
            pc += 2;

            break;

        case PUSH_OBJ:
            stack[sc] = obj_table[iseq[pc + 1]];
            sc += 1;
            pc += 2;

            break;

        case PUSH_CONST:
            stack[sc] = const_table[iseq[pc + 1]];
            sc += 1;
            pc += 2;

            break;

        case SET_OBJ:
            sc -= 1;
            obj_table[iseq[pc + 1]] = stack[sc];
            pc += 2;

            break;

        case ADD:

            break;

        case SUB:

            break;

        case MUL:

            break;

        case DIV:

            break;

        case MOD:

            break;

        case EQUAL:

            break;

        case NOT_EQUAL:

            break;

        case GREATER_THAN:

            break;

        case GREATER_THAN_OR_EQUAL:

            break;

        case LESS_THAN:

            break;

        case LESS_THAN_OR_EQUAL:

            break;

        case AND:

            break;

        case OR:

            break;

        case NOT:

            break;

        case INPUT:

            break;

        case OUT:
            sc -= 1;
            stack[sc].Out();
            pc += 1;

            break;

        case YAY:
            printf("Yay\n");
            pc += 1;

            break;

        case JUMP:
            sc -= 1;
            pc += stack[sc].GetInt();

            break;

        default:
            break;
        }
    }
}
