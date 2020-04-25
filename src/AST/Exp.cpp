//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::ast;

std::string Exp::Node2Str(uint64_t indent)
{
	if (std::holds_alternative<std::unique_ptr<Value>>(value))
		return std::get<std::unique_ptr<Value>>(value)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<BOperator>>(value))
		return std::get<std::unique_ptr<BOperator>>(value)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<UOperator>>(value))
		return std::get<std::unique_ptr<UOperator>>(value)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<Func>>(value))
		return std::get<std::unique_ptr<Func>>(value)->Node2Str(indent);

	return "";
}
