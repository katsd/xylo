//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::node;

std::string Value::Node2Str(uint64_t indent)
{
	if (std::holds_alternative<std::unique_ptr<Int>>(value))
		return std::get<std::unique_ptr<Int>>(value)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<Float>>(value))
		return std::get<std::unique_ptr<Float>>(value)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<String>>(value))
		return std::get<std::unique_ptr<String>>(value)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<Variable>>(value))
		return std::get<std::unique_ptr<Variable>>(value)->Node2Str(indent);

	return "";
}


