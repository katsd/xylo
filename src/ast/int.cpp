//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "ast/ast.hpp"

using namespace xylo::ast;

std::string Int::Node2Str(uint64_t indent)
{
	return IndentStr(indent) + std::to_string(value) + "\n";
}
