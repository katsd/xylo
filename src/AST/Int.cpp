//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::node;

std::string Int::Node2Str(uint64_t indent)
{
	return IndentStr(indent) + std::to_string(value) + "\n";
}