//
// obj.cpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#include <cmath>

#include "vm.hpp"

void VM::Obj::Out()
{
    switch (type)
    {
    case ObjType::INT:
        printf("%ld\n", value.ival);
        break;

    case ObjType::FLOAT:
        printf("%lf\n", value.dval);
        break;

    default:
        break;
    }
}

bool VM::Obj::Comp(Obj l, Obj r)
{
    if (l.type != r.type)
        return false;

    switch (l.type)
    {
    case ObjType::INT:
        return l.value.ival == r.value.ival;

    case ObjType::FLOAT:
        return abs(l.value.dval - r.value.dval) < 1e-15;

    case ObjType::INST:
        return l.value.inst == r.value.inst;
    }
}
