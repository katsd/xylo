//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _FLOAT_HPP_
#define _FLOAT_HPP_

#include <string>

#include "node.hpp"
#include "token/source_pos.hpp"

namespace xylo::ast
{
struct Float : Node
{
	double value;

	Float(double value, SourcePos pos)
		: Node(pos), value(value)
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_FLOAT_HPP_
