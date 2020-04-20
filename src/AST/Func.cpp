//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::node;

std::string Func::Node2Str(uint64_t indent)
{
	std::string res = IndentStr(indent) + name + "\n";

	for (auto& arg : args)
		res += arg->Node2Str(indent + 1);

	return res;
}
