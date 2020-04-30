//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "ast/ast.hpp"

using namespace xylo::ast;

std::string Variable::Node2Str(uint64_t indent)
{
	return IndentStr(indent) + name + "\n";
}
