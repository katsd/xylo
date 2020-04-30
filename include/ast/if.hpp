//
// Created by Katsu Matsuda on 2020/04/20.
//

#ifndef _IF_HPP_
#define _IF_HPP_

#include <memory>

#include "node.hpp"
#include "exp.hpp"
#include "token/source_pos.hpp"

namespace xylo::ast
{
/*
 * if Exp Stmt
 * if Exp Stmt else Stmt
 */
struct If : Node
{
	std::unique_ptr<Exp> exp;

	std::unique_ptr<Stmt> stmt;

	std::unique_ptr<Stmt> stmt_else;

	If(std::unique_ptr<Exp> exp, std::unique_ptr<Stmt> stmt, std::unique_ptr<Stmt> stmt_else, SourcePos pos)
		: Node(pos), exp(std::move(exp)), stmt(std::move(stmt)), stmt_else(std::move(stmt_else))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_IF_HPP_
