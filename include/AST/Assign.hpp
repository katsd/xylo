//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _ASSIGN_HPP_
#define _ASSIGN_HPP_

#include <memory>

#include "Exp.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
/*
 * Variable = Exp
 */
struct Assign
{
	std::unique_ptr<Variable> var;

	std::unique_ptr<Exp> exp;

	SourcePos pos;
};
}

#endif //_ASSIGN_HPP_
