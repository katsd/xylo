//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <memory>
#include <vector>

#include "Node.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Stmt;

/*
 * { Stmt Stmt ... Stmt }
 */
struct Block : Node
{
	std::vector<std::unique_ptr<Stmt>> stmts;

	Block(std::vector<std::unique_ptr<Stmt>> stmts, SourcePos pos)
		: stmts(std::move(stmts)), Node(pos)
	{
	}

	std::string Node2Str() override
	{
		return "Stmt";
	}

};
}

#endif //_BLOCK_HPP_
