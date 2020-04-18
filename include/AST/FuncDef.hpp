//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _FUNCDEF_HPP_
#define _FUNCDEF_HPP_

#include <string>
#include <vector>

#include "Variable.hpp"
#include "Stmt.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct FuncDef
{
	std::string name;

	std::vector<Variable> args;

	Stmt stmt;

	SourcePos pos;
};
}

#endif //_FUNCDEF_HPP_
