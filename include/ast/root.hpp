//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _ROOT_HPP_
#define _ROOT_HPP_

#include <variant>
#include <vector>
#include <memory>

#include "node.hpp"
#include "stmt.hpp"
#include "func_def.hpp"

namespace xylo::ast
{
struct Root : Node
{
	typedef std::variant<
		std::unique_ptr<Stmt>,
		std::unique_ptr<FuncDef>
	> RootStmt;

	std::vector<RootStmt> stmts;

	Root(std::vector<RootStmt> stmts, SourcePos pos)
		: Node(pos), stmts(std::move(stmts))
	{
	}

	Root()
		: Node(SourcePos()), stmts(std::vector<RootStmt>())
	{

	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_ROOT_HPP_
