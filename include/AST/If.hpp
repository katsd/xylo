//
// Created by Katsu Matsuda on 2020/04/20.
//

#ifndef _IF_HPP_
#define _IF_HPP_

#include <memory>

#include "Node.hpp"
#include "Exp.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
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
		: exp(std::move(exp)), stmt(std::move(stmt)), stmt_else(std::move(stmt_else)), Node(pos)
	{
	}

	std::string Node2Str() override
	{
		return "If";
	}
};
}

#endif //_IF_HPP_
