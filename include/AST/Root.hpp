//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _ROOT_HPP_
#define _ROOT_HPP_

#include <variant>
#include <vector>
#include <memory>

#include "Node.hpp"
#include "Stmt.hpp"
#include "FuncDef.hpp"

namespace xylo::node
{
struct Root : Node
{
	typedef std::variant<
		std::unique_ptr<Stmt>,
		std::unique_ptr<FuncDef>
	> RootStmt;

	std::vector<RootStmt> stmts;

	Root(std::vector<RootStmt> stmts, SourcePos pos)
		: stmts(std::move(stmts)), Node(pos)
	{
	}

	Root()
		: stmts(std::vector<RootStmt>()), Node(SourcePos())
	{

	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_ROOT_HPP_
