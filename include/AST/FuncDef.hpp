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

namespace xylo::node
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
		: name(std::move(name)), args(std::move(args)), stmt(std::move(stmt)), Node(pos)
	{
	}

	std::string Node2Str() override
	{
		return "FuncDef";
	}
};
}

#endif //_FUNCDEF_HPP_
