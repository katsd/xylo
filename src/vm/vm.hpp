//
// vm.hpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#ifndef VM_VM_HPP_
#define VM_VM_HPP_

#include <stdio.h>
#include <memory>
#include <vector>

class VM
{
public:
    struct Obj;

private:
    enum ObjType
    {
        INT,
        FLOAT,
    };

    union Val {
        long ival;

        double dval;

        Val() {}
        Val(long i) { ival = i; }
        Val(double d) { dval = d; }
    };

    static constexpr unsigned long obj_table_size = 1024 * 128;

    static constexpr unsigned long stack_size = 1024 * 128;

    std::vector<unsigned long> iseq;

    std::vector<Obj> const_table;

    inline Obj GetStack(unsigned long &sc, const std::unique_ptr<Obj[]> &stack)
    {
        if (sc == 0)
            return Obj();
        return stack[--sc];
    }

    inline void PushStack(unsigned long &sc, const std::unique_ptr<Obj[]> &stack, const Obj &obj)
    {
        if (sc == stack_size - 1)
            return;
        stack[sc++] = obj;
    }

public:
    struct Obj
    {
    private:
        ObjType type;

        Val value;

    public:
        Obj()
        {
            type = INT;
            value.ival = 0;
        };

        Obj(int val)
        {
            type = INT;
            value.ival = val;
        }

        Obj(long val)
        {
            type = INT;
            value.ival = val;
        };

        Obj(double val)
        {
            type = FLOAT;
            value.dval = val;
        };

        inline ObjType Type()
        {
            return type;
        };

        void SetInt(int val)
        {
            type = INT;

            value.ival = val;
        };

        void SetFloat(double val)
        {
            type = FLOAT;

            value.dval = val;
        };

        int GetInt()
        {
            switch (type)
            {
            case INT:
                return value.ival;

            case FLOAT:
                return value.dval;

            default:
                break;
            }

            return 0;
        };

        double GetFloat()
        {
            switch (type)
            {
            case INT:
                return value.ival;

            case FLOAT:
                return value.dval;

            default:
                break;
            }

            return 0.0;
        };

        void Out();
    };

    struct State
    {
    public:
        bool success;

        Obj res;

        State(Obj res)
        {
            this->success = true;
            this->res = res;
        }

        State(bool success, Obj res)
        {
            this->success = success;
            this->res = res;
        }
    };

    enum Inst
    {
        PUSH,
        PUSH_OBJ,
        PUSH_CONST,
        SET_OBJ,

        BOPE,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        EQUAL,
        NOT_EQUAL,
        GREATER_THAN,
        GREATER_THAN_OR_EQUAL,
        LESS_THAN,
        LESS_THAN_OR_EQUAL,
        AND,
        OR,
        NOT,

        INPUT,
        OUT,

        YAY,

        JUMP,
        JUMP_IF,

        START,
        END,
    };

    VM(std::vector<unsigned long> iseq, std::vector<Obj> const_table)
    {
        this->iseq = iseq;
        this->const_table = const_table;
    };

    State Run();

    State Run(unsigned long startIndex);
};

#endif /* VM_VM_HPP_ */
