//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _ASSIGN_HPP_
#define _ASSIGN_HPP_

#include "Exp.hpp"

namespace xylo::node
{
struct Assign
{
	Variable var;

	Exp exp;
};
}

#endif //_ASSIGN_HPP_
