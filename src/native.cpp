//
// Created by Katsu Matsuda on 2020/04/16.
//

#include "native.hpp"

std::vector<Native::Func> Native::funcs;

void Native::Init()
{
    Native::funcs.clear();
}

void
Native::AddFunc(VM::Obj (* func)(std::unique_ptr<VM::Obj[]>& args, unsigned long arg_num), std::string name, unsigned long arg_num)
{
    Native::funcs.push_back({ func, std::move(name), arg_num });
}

std::string Native::StandardLibraryCode()
{
    Init();

    std::string code;

    for (int i = 0; i < Native::funcs.size(); i++)
    {
        auto func = Native::funcs[i];
        code += "\nfunc _native " + std::to_string(i) + " ";
        code += func.func_name + "(";

        for (int j = 0; j < func.arg_num; j++)
        {
            code += "_" + std::to_string(j);
            if (j < func.arg_num - 1)
                code += ",";
        }

        code += "){}\n";
    }

    return code;
}

VM::Obj Native::Call(unsigned long func_id, std::unique_ptr<VM::Obj[]>& args)
{
    return Native::funcs[func_id].func(args, Native::funcs[func_id].arg_num);
}
