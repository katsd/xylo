//
// Created by Katsu Matsuda on 2020/04/30.
//

#include <string>

#include "xylo.hpp"
#include "cxylo.h"

void* CreateXylo(const char* source)
{
	auto* xylo = new Xylo{ std::string(source) };

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
AddXyloFunc(void* xylo, const char* func_name, unsigned long arg_num, CObj (* func)(CObj args[], unsigned long arg_num))
{
	native::Native::AddFunc(func, std::string(func_name), arg_num);
}