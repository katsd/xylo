//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::ast;

std::string Block::Node2Str(uint64_t indent)
{
	std::string indent_str = IndentStr(indent);

	std::string res;

	res += indent_str + "{\n";

	for (auto& stmt : stmts)
		res += stmt->Node2Str(indent + 1);

	res += indent_str + "}\n";

	return res;
}


