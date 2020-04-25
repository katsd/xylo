//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _REPEAT_HPP_
#define _REPEAT_HPP_

#include <memory>

#include "Node.hpp"
#include "Exp.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::ast
{
/*
 * repeat Exp Stmt
 */
struct Repeat : Node
{
	std::unique_ptr<Exp> time;

	std::unique_ptr<Stmt> stmt;

	Repeat(std::unique_ptr<Exp> time, std::unique_ptr<Stmt> stmt, SourcePos pos)
		: Node(pos), time(std::move(time)), stmt(std::move(stmt))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_REPEAT_HPP_
