//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _EXP_HPP_
#define _EXP_HPP_

#include <variant>
#include <memory>

#include "node.hpp"
#include "value.hpp"
#include "boperator.hpp"
#include "uoperator.hpp"
#include "func.hpp"

namespace xylo::ast
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
		: Node(pos), value(std::move(value))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_EXP_HPP_
