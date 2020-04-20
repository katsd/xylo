//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _FLOAT_HPP_
#define _FLOAT_HPP_

#include <string>

#include "Node.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Float : Node
{
	double value;

	Float(double value, SourcePos pos)
		: value(value), Node(pos)
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_FLOAT_HPP_
