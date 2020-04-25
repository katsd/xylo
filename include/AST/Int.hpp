//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _INT_HPP_
#define _INT_HPP_

#include <cstdint>
#include <string>

#include "Node.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::ast
{
struct Int : Node
{
	int64_t value;

	Int(int64_t value, SourcePos pos)
		: Node(pos), value(value)
	{
	}

	std::string Node2Str(uint64_t indent) override;
};

}

#endif //_INT_HPP_
