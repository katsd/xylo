//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::node;

std::string Return::Node2Str(uint64_t indent)
{
	std::string res = IndentStr(indent) + "return\n";

	res += exp->Node2Str(indent + 1);

	return res;
}