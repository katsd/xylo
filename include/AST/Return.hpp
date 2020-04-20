//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _RETURN_HPP_
#define _RETURN_HPP_

#include <memory>

#include "Node.hpp"
#include "Exp.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Return : Node
{
	std::unique_ptr<Exp> exp;

	Return(std::unique_ptr<Exp> exp, SourcePos pos)
		: exp(std::move(exp)), Node(pos)
	{
	}

	std::string Node2Str() override
	{
		return "Return";
	}
};
}

#endif //_RETURN_HPP_
