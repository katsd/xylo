//
// vm.cpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#include "vm.hpp"

VM::VM(std::vector<unsigned long> iseq, std::vector<Obj> const_table)
{
    this->iseq = iseq;
    this->const_table = const_table;

    global_obj_table = std::unique_ptr<Obj[]>(new Obj[obj_table_size]);

    func_start_idx = std::vector<unsigned long>();

    for (int i = 1; i < iseq.size(); i++)
    {
        if (iseq[i] == Inst::START)
            func_start_idx.push_back(i);
    }
}

VM::Result VM::Init()
{
    return Run(0);
}

VM::Result VM::RunFunc()
{
    return Run(func_start_idx[0]);
}

VM::Result VM::RunFunc(unsigned long func_id)
{
    if (func_id >= func_start_idx.size())
        return Result(false, Obj());

    return Run(func_start_idx[func_id]);
}

VM::Result VM::Run(unsigned long start_idx)
{
    std::unique_ptr<Obj[]> obj_table(new Obj[obj_table_size]);

    std::unique_ptr<Obj[]> stack(new Obj[stack_size]);

    unsigned long pc = start_idx;

    unsigned long sc = 0;

    unsigned long obj_min_idx = 0;

    long func_level = 0;

    if (iseq[pc] != Inst::START)
        return Result(false, Obj());

    PushStack(sc, stack, Obj(Inst::START));

    pc += 1;

    while (true)
    {
        unsigned long inst = iseq[pc];

        if (inst == Inst::END)
            break;

        switch (inst)
        {
        case Inst::START:
            pc += 1;

            break;

        case Inst::PUSH:
            PushStack(sc, stack, Obj((long)iseq[pc + 1]));
            pc += 2;

            break;

        case Inst::PUSH_TOP:
            if (sc < 1)
                break;

            PushStack(sc, stack, stack[sc]);
            pc += 1;

            break;

        case Inst::PUSH_OBJ:
            PushStack(sc, stack, obj_table[iseq[pc + 1] + obj_min_idx]);
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

        case Inst::PUSH_START:
            PushStack(sc, stack, Obj(Inst::START));
            pc += 1;

            break;

        case Inst::SET_OBJ:
            obj_table[iseq[pc + 1] + obj_min_idx] = GetStack(sc, stack);
            pc += 2;

            break;

        case Inst::SET_GLOBAL_OBJ:
            global_obj_table[iseq[pc + 1]] = GetStack(sc, stack);
            pc += 2;

            break;

        case Inst::POP:
            GetStack(sc, stack);
            pc += 1;

            break;

        case Inst::POP_TO_START:
            while (true)
            {
                bool brk = false;
                if (sc == 0)
                    brk = true;

                Inst ist = GetStack(sc, stack).GetInst();

                if (ist == Inst::START)
                {
                    break;
                }

                if (brk)
                    break;
            }

            pc += 1;

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

            case Inst::BAND:
                obj.SetInt(left.GetInt() & right.GetInt());

                break;

            case Inst::BOR:
                obj.SetInt(left.GetInt() | right.GetInt());

                break;

            case Inst::BXOR:
                obj.SetInt(left.GetInt() ^ right.GetInt());

                break;

            case Inst::EQUAL:
                obj.SetInt(Obj::Comp(left, right) ? 1 : 0);

                break;

            case Inst::NOT_EQUAL:
                obj.SetInt(Obj::Comp(left, right) ? 0 : 1);

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

        case Inst::BNOT:
        {
            Obj left = GetStack(sc, stack);

            PushStack(sc, stack, Obj(~left.GetInt()));

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

        case Inst::JUMP2:
            pc = GetStack(sc, stack).GetInt();

            break;

        case Inst::JUMP_IF:
            if (GetStack(sc, stack).GetInt() > 0)
            {
                pc = iseq[pc + 1];
            }
            else
            {
                pc += 2;
            }

            break;

        case Inst::PUSH_OBJ_MIN_IDX:
            PushStack(sc, stack, Obj((long)obj_min_idx));
            pc += 1;

            break;

        case Inst::ADD_OBJ_MIN_IDX:
            obj_min_idx += iseq[pc + 1];
            pc += 2;

            break;

        case Inst::SET_OBJ_MIN_IDX:
            obj_min_idx = GetStack(sc, stack).GetInt();
            pc += 1;

            break;

        case Inst::ICR_FUNC_LEVEL:
            func_level += 1;
            pc += 1;

            break;

        case Inst::DCR_FUNC_LEVEL:
            func_level -= 1;
            pc += 1;

            break;

        default:
            printf("[Error] undefined instruction : #%ld %ld\n", pc, inst);
            return Result(false, Obj());

            break;
        }

        if (func_level == -1)
            break;
    }

    return Result(GetStack(sc, stack));
}

void VM::OutIseq()
{
    unsigned long pc = 0;

    while (pc < iseq.size())
    {
        printf("#%ld ", pc);

        unsigned long inst = iseq[pc];

        switch (inst)
        {
        case Inst::START:
            printf("START\n");
            pc += 1;

            break;

        case Inst::END:
            printf("END\n");
            pc += 1;

            break;

        case Inst::PUSH:
            printf("PUSH %ld\n", iseq[pc + 1]);
            pc += 2;

            break;

        case Inst::PUSH_TOP:
            printf("PUSH_TOP\n");
            pc += 1;

            break;

        case Inst::PUSH_OBJ:
            printf("PUSH_OBJ %ld\n", iseq[pc + 1]);
            pc += 2;

            break;

        case Inst::PUSH_GLOBAL_OBJ:
            printf("PUSH_GLOBAL_OBJ %ld\n", iseq[pc + 1]);
            pc += 2;

            break;

        case Inst::PUSH_CONST:
            printf("PUSH_CONST %ld\n", iseq[pc + 1]);
            pc += 2;

            break;

        case Inst::PUSH_ZERO:
            printf("PUSH_ZERO\n");
            pc += 1;

            break;

        case Inst::PUSH_ONE:
            printf("PUSH_ONE\n");
            pc += 1;

            break;

        case Inst::PUSH_START:
            printf("PUSH_START\n");
            pc += 1;

            break;

        case Inst::SET_OBJ:
            printf("SET_OBJ %ld\n", iseq[pc + 1]);
            pc += 2;

            break;

        case Inst::SET_GLOBAL_OBJ:
            printf("SET_GLOBAL_OBJ %ld\n", iseq[pc + 1]);
            pc += 2;

            break;

        case Inst::POP:
            printf("POP\n");
            pc += 1;

            break;

        case Inst::POP_TO_START:
            printf("POP_TO_START\n");
            pc += 1;

            break;

        case Inst::BOPE:
        {
            unsigned long ope = iseq[pc + 1];

            switch (ope)
            {
            case Inst::ADD:
                printf("ADD\n");

                break;

            case Inst::SUB:
                printf("SUB\n");

                break;

            case Inst::MUL:
                printf("MUL\n");

                break;

            case Inst::DIV:
                printf("DIV\n");

                break;

            case Inst::MOD:
                printf("MOD\n");

                break;

            case Inst::EQUAL:
                printf("EQUAL\n");

                break;

            case Inst::NOT_EQUAL:
                printf("NOT_EQUAL\n");

                break;

            case Inst::GREATER_THAN:
                printf("GREATER_THAN\n");

                break;

            case Inst::GREATER_THAN_OR_EQUAL:
                printf("GREATER_THAN_EQUAL\n");

                break;

            case Inst::LESS_THAN:
                printf("LESS_THAN\n");

                break;

            case Inst::LESS_THAN_OR_EQUAL:
                printf("LESS_THAN_OR_EQUAL\n");

                break;

            case Inst::AND:
                printf("ADD\n");

                break;

            case Inst::OR:
                printf("OR\n");

                break;

            default:
                break;
            }

            pc += 2;
        }

        break;

        case Inst::NOT:
            printf("NOT\n");
            pc += 1;

            break;

        case Inst::INPUT:
            printf("INPUT\n");
            pc += 1;

            break;

        case Inst::OUT:
            printf("OUT\n");
            pc += 1;

            break;

        case Inst::YAY:
            printf("YAY\n");
            pc += 1;

            break;

        case Inst::JUMP:
            printf("JUMP %ld\n", iseq[pc + 1]);
            pc += 2;

            break;

        case Inst::JUMP2:
            printf("JUMP2\n");
            pc += 1;

            break;

        case Inst::JUMP_IF:
            printf("JUMP_IF %ld\n", iseq[pc + 1]);
            pc += 2;

            break;

        case Inst::PUSH_OBJ_MIN_IDX:
            printf("PUSH_OBJ_MIN_IDX\n");
            pc += 1;

            break;

        case Inst::ADD_OBJ_MIN_IDX:
            printf("ADD_OBJ_MIN_IDX %ld\n", iseq[pc + 1]);
            pc += 2;

            break;

        case Inst::SET_OBJ_MIN_IDX:
            printf("SET_OBJ_MIN_IDX\n");
            pc += 1;

            break;

        case Inst::ICR_FUNC_LEVEL:
            printf("ICR_FUNC_LEVEL\n");
            pc += 1;

            break;

        case Inst::DCR_FUNC_LEVEL:
            printf("DCR_FUNC_LEVEL\n");
            pc += 1;

            break;

        default:
            printf("[Error] undefined instruction : #%ld %ld\n", pc, inst);
            return;
        }
    }
}
