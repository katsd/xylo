//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _FOR_HPP_
#define _FOR_HPP_

#include <memory>

#include "Variable.hpp"
#include "Exp.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Stmt;

struct For
{
	Variable var;

	std::unique_ptr<Exp> time;

	std::unique_ptr<Stmt> stmt;

	SourcePos pos;
};
}

#endif //_FOR_HPP_
