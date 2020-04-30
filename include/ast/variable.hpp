//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _VARIABLE_HPP_
#define _VARIABLE_HPP_

#include <string>

#include "node.hpp"
#include "token/source_pos.hpp"

namespace xylo::ast
{
struct Variable : Node
{
	std::string name;

	Variable(std::string name, SourcePos pos)
		: Node(pos), name(std::move(name))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_VARIABLE_HPP_
