//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _FUNC_HPP_
#define _FUNC_HPP_

#include <string>
#include <vector>

#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Exp;

struct Func
{
	std::string name;

	std::vector<Exp> args;

	SourcePos pos;
};
}

#endif //_FUNC_HPP_
