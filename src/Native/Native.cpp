//
// Created by Katsu Matsuda on 2020/04/27.
//

#include "Native/Native.hpp"

using namespace xylo;
using namespace xylo::native;

std::vector<Native::Func> Native::funcs;

void Native::Init()
{
	funcs.clear();
}

void Native::AddFunc(const Func& func)
{
	AddFunc(func.func, func.func_name, func.arg_num);
}

void Native::AddFunc(vm::Obj (* func)(std::unique_ptr<vm::Obj[]>&, uint64_t), std::string name, uint64_t arg_num)
{
	funcs.push_back({ func, std::move(name), arg_num });
}

std::string Native::StandardLibraryCode()
{
	Init();

	std::string code;

	for (int i = 0; i < funcs.size(); i++)
	{
		auto func = funcs[i];
		code += "@native(" + std::to_string(i) + ")\n";
		code += "func " + func.func_name + "(";

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

vm::Obj Native::Call(uint64_t func_id, std::unique_ptr<vm::Obj[]>& args)
{
	auto func = funcs[func_id];
	return func.func(args, func.arg_num);
}