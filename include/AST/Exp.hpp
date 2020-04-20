//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _EXP_HPP_
#define _EXP_HPP_

#include <variant>
#include <memory>

#include "Node.hpp"
#include "Value.hpp"
#include "BOperator.hpp"
#include "UOperator.hpp"
#include "Func.hpp"

namespace xylo::node
{
struct Exp : Node
{
	typedef std::variant<
		std::unique_ptr<Value>,
		std::unique_ptr<BOperator>,
		std::unique_ptr<UOperator>,
		std::unique_ptr<Func>
	> ExpType;

	ExpType value;

	Exp(ExpType value, SourcePos pos)
		: value(std::move(value)), Node(pos)
	{
	}

	std::string Node2Str() override
	{
		return "Exp";
	}
};
}

#endif //_EXP_HPP_
