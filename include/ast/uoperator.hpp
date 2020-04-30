//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _UOPERATOR_HPP_
#define _UOPERATOR_HPP_

#include <memory>

#include "node.hpp"
#include "token/source_pos.hpp"

namespace xylo::ast
{
enum UOperatorType
{
	MINUS,
	NOT,
	BIN_NOT,
};

struct UOperator : Node
{
	UOperatorType type;

	std::unique_ptr<Exp> exp;

	UOperator(UOperatorType type, std::unique_ptr<Exp> exp, SourcePos pos)
		: Node(pos), type(type), exp(std::move(exp))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_UOPERATOR_HPP_
