//
// vm.cpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#include "vm.hpp"

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
            PushStack(sc, stack, Obj((long)iseq[pc + 1]));
            pc += 2;

            break;

        case PUSH_OBJ:
            PushStack(sc, stack, obj_table[iseq[pc + 1]]);
            pc += 2;

            break;

        case PUSH_CONST:
            PushStack(sc, stack, const_table[iseq[pc + 1]]);
            pc += 2;

            break;

        case SET_OBJ:
            obj_table[iseq[pc + 1]] = GetStack(sc, stack);
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
            GetStack(sc, stack).Out();
            pc += 1;

            break;

        case YAY:
            printf("Yay\n");
            pc += 1;

            break;

        case JUMP:
            pc = GetStack(sc, stack).GetInt();

            break;

        case JUMP_IF:
            if (GetStack(sc, stack).GetInt() > 0)
            {
                pc = GetStack(sc, stack).GetInt();
            }
            else
            {
                GetStack(sc, stack);
                pc += 1;
            }

        default:
            break;
        }
    }
}
