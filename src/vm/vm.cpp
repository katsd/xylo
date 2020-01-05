//
// vm.cpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#include "vm.hpp"

VM::State VM::Run()
{
    return Run(0);
}

VM::State VM::Run(unsigned long startIndex)
{
    std::unique_ptr<Obj[]> obj_table(new Obj[obj_table_size]);

    std::unique_ptr<Obj[]> stack(new Obj[stack_size]);

    unsigned long pc = startIndex;

    unsigned long sc = 0;

    if (iseq[pc] != Inst::START)
        return State(false, Obj());

    PushStack(sc, stack, Obj(Inst::START));

    pc += 1;

    while (true)
    {
        unsigned long inst = iseq[pc];

        if (inst == Inst::END)
            break;

        if (inst == Inst::START)
            break;

        switch (inst)
        {
        case Inst::PUSH:
            PushStack(sc, stack, Obj((long)iseq[pc + 1]));
            pc += 2;

            break;

        case Inst::PUSH_OBJ:
            PushStack(sc, stack, obj_table[iseq[pc + 1]]);
            pc += 2;

            break;

        case Inst::PUSH_GLOBAL_OBJ:
            PushStack(sc, stack, global_obj_table[iseq[pc + 1]]);
            pc += 2;

            break;

        case Inst::PUSH_CONST:
            PushStack(sc, stack, const_table[iseq[pc + 1]]);
            pc += 2;

            break;

        case Inst::PUSH_ZERO:
            PushStack(sc, stack, Obj(0));
            pc += 1;

            break;

        case Inst::PUSH_ONE:
            PushStack(sc, stack, Obj(1));
            pc += 1;

            break;

        case Inst::SET_OBJ:
            obj_table[iseq[pc + 1]] = GetStack(sc, stack);
            pc += 2;

            break;

        case Inst::SET_GLOBAL_OBJ:
            global_obj_table[iseq[pc + 1]] = GetStack(sc, stack);
            pc += 2;

            break;

        case Inst::POP:
            GetStack(sc, stack);

            break;

        case Inst::POP_TO_START:
            while (inst >= 0)
            {
                if (GetStack(sc, stack).GetInst() == Inst::START)
                    break;
            }

            break;

        case Inst::BOPE:
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
            case Inst::ADD:
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

            case Inst::SUB:
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

            case Inst::MUL:
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

            case Inst::DIV:
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

            case Inst::MOD:
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

            case Inst::EQUAL:
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

            case Inst::NOT_EQUAL:
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

            case Inst::GREATER_THAN:
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

            case Inst::GREATER_THAN_OR_EQUAL:
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

            case Inst::LESS_THAN:
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

            case Inst::LESS_THAN_OR_EQUAL:
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

            case Inst::AND:
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

            case Inst::OR:
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

        case Inst::NOT:
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

        case Inst::INPUT:
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

        case Inst::OUT:
            GetStack(sc, stack).Out();
            pc += 1;

            break;

        case Inst::YAY:
            printf("Yay\n");
            pc += 1;

            break;

        case Inst::JUMP:
            pc = iseq[pc + 1];

            break;

        case Inst::JUMP_IF:
            if (GetStack(sc, stack).GetInt() > 0)
            {
                pc = iseq[pc + 1];
            }
            else
            {
                GetStack(sc, stack);
                pc += 2;
            }

            break;

        default:
            printf("[Error] undefined instruction : #%ld %ld\n", pc, inst);
            return State(false, Obj());

            break;
        }
    }

    return State(GetStack(sc, stack));
}
