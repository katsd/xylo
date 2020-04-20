//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _STRING_HPP_
#define _STRING_HPP_

#include <string>

#include "Node.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct String : Node
{
	std::string value;

	String(std::string value, SourcePos pos)
		: value(std::move(value)), Node(pos)
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_STRING_HPP_
