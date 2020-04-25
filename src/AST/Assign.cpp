//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::ast;

std::string Assign::Node2Str(uint64_t indent)
{
	std::string res = IndentStr(indent) + "assign\n";

	res += var->Node2Str(indent + 1);

	res += exp->Node2Str(indent + 1);

	return res;
}
