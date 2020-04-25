//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _FUNCDEF_HPP_
#define _FUNCDEF_HPP_

#include <memory>
#include <string>
#include <vector>

#include "Node.hpp"
#include "Variable.hpp"
#include "Stmt.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::ast
{
/*
 * func Symbol::NAME(Symbol::Name, .., Symbol::NAME)
 */
struct FuncDef : Node
{
	std::string name;

	std::vector<std::unique_ptr<Variable>> args;

	std::unique_ptr<Stmt> stmt;

	FuncDef(std::string name, std::vector<std::unique_ptr<Variable>> args, std::unique_ptr<Stmt> stmt, SourcePos pos)
		: Node(pos), name(std::move(name)), args(std::move(args)), stmt(std::move(stmt))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_FUNCDEF_HPP_
