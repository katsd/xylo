//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _VARIABLE_HPP_
#define _VARIABLE_HPP_

#include <string>

#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Variable
{
	std::string name;

	SourcePos pos;
};
}

#endif //_VARIABLE_HPP_
