//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _FUNC_DEF_HPP_
#define _FUNC_DEF_HPP_

#include <memory>
#include <string>
#include <vector>

#include "node.hpp"
#include "variable.hpp"
#include "stmt.hpp"
#include "token/source_pos.hpp"

namespace xylo::ast
{
/*
 * @native(INT)
 * func Symbol::NAME(Symbol::NAME, .., Symbol::NAME)
 */
struct FuncDef : Node
{
	std::string name;

	bool is_native_func;

	uint64_t native_func_id;

	std::vector<std::unique_ptr<Variable>> args;

	std::unique_ptr<Stmt> stmt;

	FuncDef(std::string name,
		bool is_native_func,
		uint64_t native_func_id,
		std::vector<std::unique_ptr<Variable>> args,
		std::unique_ptr<Stmt> stmt,
		SourcePos pos)
		: Node(pos),
		  name(std::move(name)),
		  is_native_func(is_native_func),
		  native_func_id(native_func_id),
		  args(std::move(args)),
		  stmt(std::move(stmt))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_FUNC_DEF_HPP_
