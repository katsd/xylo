//
// obj.cpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#include "vm.hpp"

void VM::Obj::SetInt(int val)
{
    type = INT;

    value.ival = val;
}

void VM::Obj::SetFloat(double val)
{
    type = FLOAT;

    value.dval = val;
}

int VM::Obj::GetInt()
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
}

double VM::Obj::GetFloat()
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
}

void VM::Obj::Out()
{
    switch (type)
    {
    case INT:
        printf("%ld\n", value.ival);
        break;

    case FLOAT:
        printf("%f\n", value.dval);
        break;

    default:
        break;
    }
}
