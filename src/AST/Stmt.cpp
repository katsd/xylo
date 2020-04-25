//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "AST/AST.hpp"

using namespace xylo::ast;

std::string Stmt::Node2Str(uint64_t indent)
{
	if (std::holds_alternative<std::unique_ptr<Block>>(stmt))
		return std::get<std::unique_ptr<Block>>(stmt)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<Assign>>(stmt))
		return std::get<std::unique_ptr<Assign>>(stmt)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<Func>>(stmt))
		return std::get<std::unique_ptr<Func>>(stmt)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<If>>(stmt))
		return std::get<std::unique_ptr<If>>(stmt)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<While>>(stmt))
		return std::get<std::unique_ptr<While>>(stmt)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<Repeat>>(stmt))
		return std::get<std::unique_ptr<Repeat>>(stmt)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<For>>(stmt))
		return std::get<std::unique_ptr<For>>(stmt)->Node2Str(indent);

	if (std::holds_alternative<std::unique_ptr<Return>>(stmt))
		return std::get<std::unique_ptr<Return>>(stmt)->Node2Str(indent);

	return "";
}
