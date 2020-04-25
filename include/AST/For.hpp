//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _FOR_HPP_
#define _FOR_HPP_

#include <memory>

#include "Node.hpp"
#include "Variable.hpp"
#include "Exp.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::ast
{
/*
 * for Variable,Exp Stmt
 */
struct For : Node
{
	std::unique_ptr<Variable> var;

	std::unique_ptr<Exp> time;

	std::unique_ptr<Stmt> stmt;

	For(std::unique_ptr<Variable> var, std::unique_ptr<Exp> time, std::unique_ptr<Stmt> stmt, SourcePos pos)
		: Node(pos), var(std::move(var)), time(std::move(time)), stmt(std::move(stmt))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_FOR_HPP_
