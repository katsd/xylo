//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _REPEAT_HPP_
#define _REPEAT_HPP_

#include <memory>

#include "Exp.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Stmt;

struct Repeat
{
	std::unique_ptr<Exp> time;

	std::unique_ptr<Stmt> stmt;

	SourcePos pos;
};
}

#endif //_REPEAT_HPP_
