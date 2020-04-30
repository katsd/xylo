//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _RETURN_HPP_
#define _RETURN_HPP_

#include <memory>

#include "node.hpp"
#include "exp.hpp"
#include "token/source_pos.hpp"

namespace xylo::ast
{
struct Return : Node
{
	std::unique_ptr<Exp> exp;

	Return(std::unique_ptr<Exp> exp, SourcePos pos)
		: Node(pos), exp(std::move(exp))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_RETURN_HPP_
