//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _VALUE_HPP_
#define _VALUE_HPP_

#include <variant>
#include <memory>

#include "node.hpp"
#include "int.hpp"
#include "float.hpp"
#include "string.hpp"
#include "variable.hpp"
#include "token/source_pos.hpp"

namespace xylo::ast
{
struct Value : Node
{
	typedef std::variant<
		std::unique_ptr<Int>,
		std::unique_ptr<Float>,
		std::unique_ptr<String>,
		std::unique_ptr<Variable>
	> ValueType;

	ValueType value;

	Value(ValueType value, SourcePos pos)
		: Node(pos), value(std::move(value))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_VALUE_HPP_


