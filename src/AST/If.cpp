//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::node;

std::string If::Node2Str(uint64_t indent)
{
	std::string res = IndentStr(indent) + "if\n";

	res += exp->Node2Str(indent + 1);

	res += stmt->Node2Str(indent + 1);

	if (stmt_else != nullptr)
	{
		res += IndentStr(indent) + "else\n";
		res += stmt_else->Node2Str(indent + 1);
	}

	return res;
}
