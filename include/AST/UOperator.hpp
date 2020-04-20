//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _UOPERATOR_HPP_
#define _UOPERATOR_HPP_

#include <memory>

#include "Node.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
enum UOperatorType
{
	MINUS,
	NOT_EQ,
};

struct UOperator : Node
{
	UOperatorType type;

	std::unique_ptr<Exp> exp;

	UOperator(UOperatorType type, std::unique_ptr<Exp> exp, SourcePos pos)
		: type(type), exp(std::move(exp)), Node(pos)
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_UOPERATOR_HPP_
