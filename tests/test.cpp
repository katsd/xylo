//
// test.cpp
// xylo
//
// Copyright (c) 2020 Katsu Matsuda. All rights reserved.
//

#include <stdio.h>
#include <string>

#include "../src/xylo.hpp"

void out(bool result)
{
    if (result)
        printf("\x1b[32mAC\x1b[39m\n");
    else
        printf("\x1b[31mWA\x1b[39m\n");
}

void eval(std::string test_name, std::string code_str, VM::Obj ans)
{
    auto eval = Xylo(code_str);

    auto res = eval.Run(2);

    printf("%s : ", test_name.c_str());

    out(res.success && VM::Obj::Comp(res.res, ans));
}

std::string in_func(std::string code_str)
{
    return "func main(){" + code_str + "}";
}

int main()
{
    eval("int", in_func("return 128"), VM::Obj(128));
    eval("float", in_func("return 2.718281828"), VM::Obj(2.718281828));

    eval("+", in_func("return 128+256"), VM::Obj(384));
    eval("-", in_func("return 128-256"), VM::Obj(-128));
    eval("*", in_func("return 128*256"), VM::Obj(32768));
    eval("/", in_func("return 128/256"), VM::Obj(0.5));
    eval("%", in_func("return 256%10"), VM::Obj(6));
    eval("&", in_func("return 11&14"), VM::Obj(10));
    eval("|", in_func("return 10|6"), VM::Obj(14));
    eval("~", in_func("return ~128"), VM::Obj(-129));
    eval("&&", in_func("return 128 == 128 && 256 == 256"), VM::Obj(1));
    eval("||", in_func("return 128 == 128 || 256 != 256"), VM::Obj(1));
    eval("!", in_func("return !(256 != 256)"), VM::Obj(1));
    eval("==", in_func("return 3.14 == 3.14"), VM::Obj(1));
    eval("!=", in_func("return 3.1415926535 != 3.1415926536"), VM::Obj(1));
    eval(">", in_func("return 256>128"), VM::Obj(1));
    eval("<", in_func("return 3.1415926535<3.1415926536"), VM::Obj(1));
    eval(">=", in_func("return 256>=128 && 128>=128"), VM::Obj(1));
    eval("<=", in_func("return 3.1415926535<=3.1415926536 && 3.1415926535<=3.1415926535"), VM::Obj(1));

    return 0;
}