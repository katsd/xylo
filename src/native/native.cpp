//
// Created by Katsu Matsuda on 2020/04/27.
//

#include "native/native.hpp"

using namespace xylo;
using namespace xylo::native;

std::vector<Native::Func> Native::funcs;

CObj ctest(CObj args[], unsigned long arg_num)
{
	puts("Yay from C Func");

	printf("%ld\n", args[0].value.ival);

	return CObj{ CObjType::INT, CObjValue{ 0 }};
}

void Native::DeleteAllFunc()
{
	funcs.clear();
}

void Native::Init()
{
	auto cur = funcs;
	funcs.clear();

	for (const auto& func : cur)
	{
		if (func.is_external)
			funcs.push_back(func);
	}
}

void Native::AddFunc(const Func& func)
{
	AddFunc(func.func, func.func_name, func.arg_num);
}

void Native::AddFunc(vm::Obj (* func)(std::unique_ptr<vm::Obj[]>&, uint64_t), std::string name, uint64_t arg_num)
{
	funcs.push_back({ func, nullptr, std::move(name), arg_num, false });
}

void Native::AddFunc(CObj (* func)(CObj*, unsigned long), std::string name, unsigned long arg_num)
{
	funcs.push_back({ nullptr, func, std::move(name), arg_num, true });
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

	if (!func.is_external)
		return func.func(args, func.arg_num);

	CObj cargs[func.arg_num];
	for (uint64_t i = 0; i < func.arg_num; i++)
	{
		switch (args[i].GetType())
		{
		case vm::ObjType::INST:
			cargs[i].type = CObjType::INT;
			cargs[i].value.ival = 0;
			break;
		case vm::ObjType::INT:
			cargs[i].type = CObjType::INT;
			cargs[i].value.ival = args[i].GetInt();
			break;
		case vm::ObjType::FLOAT:
			cargs[i].type = CObjType::FLOAT;
			cargs[i].value.dval = args[i].GetFloat();
			break;
		case vm::ObjType::STRING:
			cargs[i].type = CObjType::STRING;
			cargs[i].value.str = args[i].GetString().c_str();
			break;
		}
	}

	auto res = func.ext_func(cargs, func.arg_num);

	switch (res.type)
	{
	case CObjType::INT:
		return vm::Obj{ (int64_t)res.value.ival };
	case CObjType::FLOAT:
		return vm::Obj{ res.value.dval };
	case CObjType::STRING:
		return vm::Obj{ std::string(res.value.str) };
	}

	return vm::Obj{};
}