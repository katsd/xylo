//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _WHILE_HPP_
#define _WHILE_HPP_

#include <string>

#include "node.hpp"
#include "exp.hpp"
#include "token/source_pos.hpp"

namespace xylo::ast
{
/*
 * while Exp Stmt
 */
struct While : Node
{
	std::unique_ptr<Exp> exp;

	std::unique_ptr<Stmt> stmt;

	While(std::unique_ptr<Exp> exp, std::unique_ptr<Stmt> stmt, SourcePos pos)
		: Node(pos), exp(std::move(exp)), stmt(std::move(stmt))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_WHILE_HPP_
