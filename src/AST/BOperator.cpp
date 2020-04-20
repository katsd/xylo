//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::node;

std::string BOperator::Node2Str(uint64_t indent)
{
	std::string indent_str = IndentStr(indent);

	std::string res = indent_str;

	switch (type)
	{
	case OR:
		res += "||";
	case AND:
		res += "&&";
	case BIN_OR :
		res += "|";
	case BIN_XOR:
		res += "^";
	case BIN_AND:
		res += "&";
	case EQUAL:
		res += "==";
	case NOT_EQUAL:
		res += "!=";
	case LESS:
		res += "<";
	case LESS_EQ:
		res += "<=";
	case GREATER:
		res += ">";
	case GREATER_EQ:
		res += ">=";
	case ADD:
		res += "+";
	case SUB:
		res += "-";
	case MUL:
		return "*";
	case DIV:
		return "/";
	case MOD:
		return "%";
	default:
		break;
	}

	res += '\n';

	res += left->Node2Str(indent + 1);
	res += right->Node2Str(indent + 1);

	return res;
}

