//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _BOPERATOR_HPP_
#define _BOPERATOR_HPP_

#include <memory>

#include "Node.hpp"
#include "Token/Token.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::ast
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
		: Node(pos), type(type), left(std::move(left)), right(std::move(right))
	{
	}

	static BOperatorType Symbol2BOperator(Symbol symbol);

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_BOPERATOR_HPP_
