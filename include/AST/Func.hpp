//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _FUNC_HPP_
#define _FUNC_HPP_

#include <memory>
#include <string>
#include <vector>

#include "Node.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
/*
 * TokenType::Name(Exp, Exp, .., Exp)
 */
struct Func : Node
{
	std::string name;

	std::vector<std::unique_ptr<Exp>> args;

	Func(std::string name, std::vector<std::unique_ptr<Exp>> args, SourcePos pos)
		: name(std::move(name)), args(std::move(args)), Node(pos)
	{
	}

	std::string Node2Str() override
	{
		return "Func";
	}

};
}

#endif //_FUNC_HPP_
