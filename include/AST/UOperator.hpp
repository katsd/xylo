//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _UOPERATOR_HPP_
#define _UOPERATOR_HPP_

#include <memory>

#include "Token/SourcePos.hpp"

namespace xylo::node
{
enum UOperatorType
{
	MINUS,
	NOT_EQ,
};

struct Exp;

struct UOperator
{
	UOperatorType type;

	std::unique_ptr<Exp> value;

	SourcePos pos;
};
}

#endif //_UOPERATOR_HPP_
