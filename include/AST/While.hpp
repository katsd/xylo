//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _WHILE_HPP_
#define _WHILE_HPP_

#include <string>

#include "Exp.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Stmt;

struct While
{
	std::unique_ptr<Exp> exp;

	std::unique_ptr<Stmt> stmt;

	SourcePos pos;
};
}

#endif //_WHILE_HPP_
