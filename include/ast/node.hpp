//
// Created by Katsu Matsuda on 2020/04/20.
//

#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <string>

#include "token/source_pos.hpp"

namespace xylo::ast
{
struct Node
{
	static constexpr uint64_t indent_size = 2;

	SourcePos pos;

	Node(SourcePos pos)
		: pos(pos)
	{
	}

	virtual ~Node()
	{
	}

	virtual std::string Node2Str(uint64_t indent) = 0;

	inline static std::string IndentStr(uint64_t indent)
	{
		return std::string(indent * indent_size, ' ');
	}
};
}

#endif //_NODE_HPP_
