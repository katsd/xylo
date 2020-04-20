//
// Created by Katsu Matsuda on 2020/04/20.
//

#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <string>

#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Node
{
	SourcePos pos;

	Node(SourcePos pos)
		: pos(pos)
	{
	}

	virtual std::string Node2Str() = 0;
};
}

#endif //_NODE_HPP_
