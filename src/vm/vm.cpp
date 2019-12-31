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

VM::State VM::Run(unsigned long startIndex)
{
    std::unique_ptr<Obj[]> obj_table(new Obj[obj_table_size]);

    std::unique_ptr<Obj[]> stack(new Obj[stack_size]);

    unsigned long pc = startIndex;

    unsigned long sc = 0;

    if (iseq[pc] != START)
        return State(false, Obj());

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

        case BOPE:
        {
            unsigned long ope = iseq[pc + 1];

            Obj right = GetStack(sc, stack);
            Obj left = GetStack(sc, stack);

            ObjType type;
            if (left.Type() == INT && right.Type() == INT)
                type = INT;
            else
                type = FLOAT;

            Obj obj;

            switch (ope)
            {
            case ADD:
                switch (type)
                {
                case INT:
                    obj.SetInt(left.GetInt() + right.GetInt());
                    break;

                case FLOAT:
                    obj.SetFloat(left.GetFloat() + right.GetFloat());
                    break;

                default:
                    break;
                }

                break;

            case SUB:
                switch (type)
                {
                case INT:
                    obj.SetInt(left.GetInt() - right.GetInt());
                    break;

                case FLOAT:
                    obj.SetFloat(left.GetFloat() - right.GetFloat());
                    break;

                default:
                    break;
                }

                break;

            case MUL:
                switch (type)
                {
                case INT:
                    obj.SetInt(left.GetInt() * right.GetInt());
                    break;

                case FLOAT:
                    obj.SetFloat(left.GetFloat() * right.GetFloat());
                    break;

                default:
                    break;
                }

                break;

            case DIV:
                switch (type)
                {
                case INT:
                    if (right.GetInt() == 0)
                    {
                        obj.SetInt(0);
                    }
                    else
                    {
                        unsigned long lv = left.GetInt();
                        unsigned long rv = right.GetInt();
                        if (lv % rv == 0)
                            obj.SetInt(lv / rv);
                        else
                            obj.SetFloat((double)lv / rv);
                    }
                    break;

                case FLOAT:
                    if (right.GetFloat() == 0)
                        obj.SetFloat(0);
                    else
                        obj.SetFloat(left.GetFloat() / right.GetFloat());
                    break;

                default:
                    break;
                }

                break;

            case MOD:
                switch (type)
                {
                case INT:
                    if (right.GetInt() == 0)
                        obj.SetInt(0);
                    else
                        obj.SetInt(left.GetInt() % right.GetInt());
                    break;

                case FLOAT:
                    obj.SetFloat(0);
                    break;

                default:
                    break;
                }

                break;

            case EQUAL:
                switch (type)
                {
                case INT:
                    obj.SetInt(left.GetInt() == right.GetInt() ? 1 : 0);
                    break;

                case FLOAT:
                    obj.SetInt(left.GetFloat() == right.GetFloat() ? 1 : 0);
                    break;

                default:
                    break;
                }

                break;

            case NOT_EQUAL:
                switch (type)
                {
                case INT:
                    obj.SetInt(left.GetInt() != right.GetInt() ? 1 : 0);
                    break;

                case FLOAT:
                    obj.SetInt(left.GetFloat() != right.GetFloat() ? 1 : 0);
                    break;

                default:
                    break;
                }

                break;

            case GREATER_THAN:
                switch (type)
                {
                case INT:
                    obj.SetInt(left.GetInt() > right.GetInt() ? 1 : 0);
                    break;

                case FLOAT:
                    obj.SetInt(left.GetFloat() > right.GetFloat() ? 1 : 0);
                    break;

                default:
                    break;
                }

                break;

            case GREATER_THAN_OR_EQUAL:
                switch (type)
                {
                case INT:
                    obj.SetInt(left.GetInt() >= right.GetInt() ? 1 : 0);
                    break;

                case FLOAT:
                    obj.SetInt(left.GetFloat() >= right.GetFloat() ? 1 : 0);
                    break;

                default:
                    break;
                }

                break;

            case LESS_THAN:
                switch (type)
                {
                case INT:
                    obj.SetInt(left.GetInt() < right.GetInt() ? 1 : 0);
                    break;

                case FLOAT:
                    obj.SetInt(left.GetFloat() < right.GetFloat() ? 1 : 0);
                    break;

                default:
                    break;
                }

                break;

            case LESS_THAN_OR_EQUAL:
                switch (type)
                {
                case INT:
                    obj.SetInt(left.GetInt() <= right.GetInt() ? 1 : 0);
                    break;

                case FLOAT:
                    obj.SetInt(left.GetFloat() <= right.GetFloat() ? 1 : 0);
                    break;

                default:
                    break;
                }

                break;

            case AND:
                switch (type)
                {
                case INT:
                    obj.SetInt((left.GetInt() > 0 && right.GetInt() > 0) ? 1 : 0);
                    break;

                case FLOAT:
                    obj.SetInt((left.GetFloat() > 0 + right.GetFloat() > 0) ? 1 : 0);
                    break;

                default:
                    break;
                }

                break;

            case OR:
                switch (type)
                {
                case INT:
                    obj.SetInt((left.GetInt() > 0 || right.GetInt() > 0) ? 1 : 0);
                    break;

                case FLOAT:
                    obj.SetInt((left.GetFloat() > 0 || right.GetFloat() > 0) ? 1 : 0);
                    break;

                default:
                    break;
                }

                break;

            default:
                break;
            }

            PushStack(sc, stack, obj);
            pc += 2;
        }

        break;

        case NOT:
        {
            Obj left = GetStack(sc, stack);

            Obj obj;

            switch (left.Type())
            {
            case INT:
                obj.SetInt(left.GetInt() <= 0);
                break;

            case FLOAT:
                obj.SetInt(left.GetFloat() <= 0);
                break;

            default:
                break;
            }

            PushStack(sc, stack, obj);
            pc += 1;
        }

        break;

        case INPUT:
        {
            double val;
            scanf("%lf", &val);

            Obj obj;

            if ((long)val == val)
                obj.SetInt((long)val);
            else
                obj.SetFloat(val);

            PushStack(sc, stack, obj);

            pc += 1;
        }

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

    return State(GetStack(sc, stack));
}
