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
		break;
	case AND:
		res += "&&";
		break;
	case BIN_OR :
		res += "|";
		break;
	case BIN_XOR:
		res += "^";
		break;
	case BIN_AND:
		res += "&";
		break;
	case EQUAL:
		res += "==";
		break;
	case NOT_EQUAL:
		res += "!=";
		break;
	case LESS:
		res += "<";
		break;
	case LESS_EQ:
		res += "<=";
		break;
	case GREATER:
		res += ">";
		break;
	case GREATER_EQ:
		res += ">=";
		break;
	case ADD:
		res += "+";
		break;
	case SUB:
		res += "-";
		break;
	case MUL:
		res += "*";
		break;
	case DIV:
		res += "/";
		break;
	case MOD:
		res += "%";
		break;
	default:
		break;
	}

	res += '\n';

	res += left->Node2Str(indent + 1);
	res += right->Node2Str(indent + 1);

	return res;
}

