//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _STRING_HPP_
#define _STRING_HPP_

#include <string>

#include "node.hpp"
#include "token/source_pos.hpp"

namespace xylo::ast
{
struct String : Node
{
	std::string value;

	String(std::string value, SourcePos pos)
		: Node(pos), value(std::move(value))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_STRING_HPP_
