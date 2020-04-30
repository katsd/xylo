//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _ASSIGN_HPP_
#define _ASSIGN_HPP_

#include <memory>

#include "node.hpp"
#include "exp.hpp"
#include "token/source_pos.hpp"

namespace xylo::ast
{
/*
 * Variable = Exp
 */
struct Assign : Node
{
	std::unique_ptr<Variable> var;

	std::unique_ptr<Exp> exp;

	Assign(std::unique_ptr<Variable> var, std::unique_ptr<Exp> exp, SourcePos pos)
		: Node(pos), var(std::move(var)), exp(std::move(exp))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_ASSIGN_HPP_
