//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::ast;

std::string UOperator::Node2Str(uint64_t indent)
{
	std::string res = IndentStr(indent);

	switch (type)
	{
	case MINUS:
		res += "-";
		break;
	case NOT:
		res += "!";
		break;
	case BIN_NOT:
		res += "~";
		break;
	default:
		break;
	}

	res += "\n";

	res += exp->Node2Str(indent + 1);

	return res;
}

