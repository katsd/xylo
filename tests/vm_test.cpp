//
// vm_test.cpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#include <stdio.h>
#include "../src/vm/vm.hpp"

int main()
{
    std::vector<unsigned long> iseq = {VM::PUSH_CONST, 0, VM::OUT, VM::END};
    std::vector<VM::Obj> const_table = {VM::Obj((long)3)};

    VM eval = VM(iseq, const_table);

    eval.Run();

    return 0;
}