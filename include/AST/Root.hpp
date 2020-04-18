//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _ROOT_HPP_
#define _ROOT_HPP_

#include <variant>
#include <vector>
#include <memory>

#include "Stmt.hpp"
#include "FuncDef.hpp"

namespace xylo::node
{
struct Root
{
	typedef std::variant<
		std::unique_ptr<Stmt>,
		std::unique_ptr<FuncDef>
	> RootStmt;

	std::vector<RootStmt> stmts;
};
}

#endif //_ROOT_HPP_
