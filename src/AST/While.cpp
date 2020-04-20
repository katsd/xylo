//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::node;

std::string While::Node2Str(uint64_t indent)
{
	std::string res = IndentStr(indent) + "while\n";

	res += exp->Node2Str(indent + 1);

	res += stmt->Node2Str(indent + 1);

	return res;
}
