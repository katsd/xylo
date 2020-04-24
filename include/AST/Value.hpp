//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _VALUE_HPP_
#define _VALUE_HPP_

#include <variant>
#include <memory>

#include "Node.hpp"
#include "Int.hpp"
#include "Float.hpp"
#include "String.hpp"
#include "Variable.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
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


