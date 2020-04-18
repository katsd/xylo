//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _BOPERATOR_HPP_
#define _BOPERATOR_HPP_

#include <memory>

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
};

struct Exp;

struct BOperator
{
	BOperatorType type;

	std::unique_ptr<Exp> left;

	std::unique_ptr<Exp> right;

	SourcePos pos;
};
}

#endif //_BOPERATOR_HPP_
