//
// Created by Katsu Matsuda on 2020/04/30.
//

#include <string>

#include "xylo.hpp"
#include "cxylo.h"

void* CreateXylo(const void* ext_xylo_instance, const char* source,
	CObj (* call_ext_func)(const void* ext_xylo_instance, const char* func_name, unsigned long arg_num, CObj args[]))
{
	native::Native::SetExtFunc(call_ext_func);

	auto* xylo = new Xylo{ std::string(source), ext_xylo_instance };

	return xylo;
}

void RunXylo(void* xylo)
{
	auto eval = (Xylo*)xylo;

	eval->Run();
}

void RunXyloFunc(void* xylo, const char* func_name)
{
	auto eval = (Xylo*)xylo;

	eval->Run(std::string(func_name), {});
}

void
AddXyloFunc(const char* func_name, unsigned long arg_num)
{
	native::Native::AddFunc(std::string(func_name), arg_num);
}

void DeleteAllXyloFunc()
{
	native::Native::DeleteAllFunc();
}