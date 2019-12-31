//
// obj.cpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#include "vm.hpp"

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
