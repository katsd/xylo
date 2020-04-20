//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::node;

BOperatorType BOperator::Symbol2BOperator(Symbol symbol)
{
	switch (symbol)
	{
	case Symbol::OR:
		return BOperatorType::OR;
	case Symbol::AND:
		return BOperatorType::AND;
	case Symbol::BIN_OR:
		return BOperatorType::BIN_OR;
	case Symbol::BIN_XOR:
		return BOperatorType::BIN_XOR;
	case Symbol::BIN_AND:
		return BOperatorType::BIN_AND;
	case Symbol::EQUAL:
		return BOperatorType::EQUAL;
	case Symbol::NOT_EQUAL:
		return BOperatorType::NOT_EQUAL;
	case Symbol::LESS:
		return LESS;
	case Symbol::LESS_EQ:
		return BOperatorType::LESS_EQ;
	case Symbol::GREATER:
		return BOperatorType::GREATER;
	case Symbol::GREATER_EQ:
		return BOperatorType::GREATER_EQ;
	case Symbol::PLUS:
		return BOperatorType::ADD;
	case Symbol::MINUS:
		return BOperatorType::SUB;
	case Symbol::MUL:
		return BOperatorType::MUL;
	case Symbol::DIV:
		return BOperatorType::DIV;
	case Symbol::MOD:
		return BOperatorType::MOD;
	default:
		return BOperatorType::NONE;
	}
}

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

