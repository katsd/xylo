//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _STRING_HPP_
#define _STRING_HPP_

#include <string>

#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct String
{
	std::string value;

	SourcePos pos;
};
}

#endif //_STRING_HPP_
