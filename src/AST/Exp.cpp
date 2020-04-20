//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::node;

std::string Exp::Node2Str(uint64_t indent)
{
	if (std::holds_alternative<std::unique_ptr<Value>>(value))
		std::get<std::unique_ptr<Value>>(value)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<BOperator>>(value))
		std::get<std::unique_ptr<Value>>(value)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<UOperator>>(value))
		std::get<std::unique_ptr<Value>>(value)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<Func>>(value))
		std::get<std::unique_ptr<Value>>(value)->Node2Str(indent);

	return "";
}
