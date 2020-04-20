//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _WHILE_HPP_
#define _WHILE_HPP_

#include <string>

#include "Node.hpp"
#include "Exp.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
/*
 * while Exp Stmt
 */
struct While : Node
{
	std::unique_ptr<Exp> exp;

	std::unique_ptr<Stmt> stmt;

	While(std::unique_ptr<Exp> exp, std::unique_ptr<Stmt> stmt, SourcePos pos)
		: exp(std::move(exp)), stmt(std::move(stmt)), Node(pos)
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_WHILE_HPP_
