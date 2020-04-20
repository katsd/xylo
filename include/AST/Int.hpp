//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _INT_HPP_
#define _INT_HPP_

#include <cstdint>
#include <string>

#include "Node.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Int : Node
{
	uint64_t value;

	Int(uint64_t value, SourcePos pos)
		: value(value), Node(pos)
	{
	}

	std::string Node2Str() override
	{
		return "Int";
	}
};

}

#endif //_INT_HPP_
