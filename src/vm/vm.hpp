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

    std::vector<unsigned long> iseq;

    std::vector<Obj> const_table;

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

        Obj(long val)
        {
            type = INT;
            value.ival = val;
        };

        Obj(double val)
        {
            type = FLOAT;
            value.dval = val;
        }

        ObjType Type();

        void SetInt(int val);

        void SetFloat(double val);

        int GetInt();

        double GetFloat();

        void Out();
    };

    enum Inst
    {
        PUSH,
        PUSH_OBJ,
        PUSH_CONST,
        SET_OBJ,

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
        END,
    };

    VM(std::vector<unsigned long> iseq, std::vector<Obj> const_table)
    {
        this->iseq = iseq;
        this->const_table = const_table;
    };

    void Run();
};

#endif /* VM_VM_HPP_ */
