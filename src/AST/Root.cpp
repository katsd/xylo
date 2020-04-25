//
// Created by Katsu Matsuda on 2020/04/20.
//

#include <AST/Root.hpp>

#include "AST/AST.hpp"

using namespace xylo::ast;

std::string Root::Node2Str(uint64_t indent)
{
	std::string res;

	for (auto& stmt : stmts)
	{
		if (std::holds_alternative<std::unique_ptr<Stmt>>(stmt))
			res += std::get<std::unique_ptr<Stmt>>(stmt)->Node2Str(indent);

		else if (std::holds_alternative<std::unique_ptr<FuncDef>>(stmt))
			res += std::get<std::unique_ptr<FuncDef>>(stmt)->Node2Str(indent);
	}

	return res;
}
