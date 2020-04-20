//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _BOPERATOR_HPP_
#define _BOPERATOR_HPP_

#include <memory>

#include "Node.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
enum BOperatorType
{
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	BIN_AND,
	BIN_OR,
	BIN_XOR,
	AND,
	OR,
	EQUAL,
	NOT_EQUAL,
	GREATER,
	GREATER_EQ,
	LESS,
	LESS_EQ,
	NONE,
};

struct Exp;

struct BOperator : Node
{
	BOperatorType type;

	std::unique_ptr<Exp> left;

	std::unique_ptr<Exp> right;

	BOperator(BOperatorType type, std::unique_ptr<Exp> left, std::unique_ptr<Exp> right, SourcePos pos)
		: type(type), left(std::move(left)), right(std::move(right)), Node(pos)
	{
	}

	static BOperatorType Symbol2BOperator(Symbol symbol)
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

	std::string Node2Str() override
	{
		return "BOperator";
	}
};
}

#endif //_BOPERATOR_HPP_
