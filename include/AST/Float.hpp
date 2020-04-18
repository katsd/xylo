//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _FLOAT_HPP_
#define _FLOAT_HPP_

#include <string>

#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Float
{
	double value;

	SourcePos pos;
};
}

#endif //_FLOAT_HPP_
