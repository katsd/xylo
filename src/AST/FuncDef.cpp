//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::ast;

std::string FuncDef::Node2Str(uint64_t indent)
{
	std::string res = IndentStr(indent) + "func " + name + "\n";

	for (auto& arg : args)
		res += arg->Node2Str(indent + 1);

	res += stmt->Node2Str(indent + 1);

	return res;
}
