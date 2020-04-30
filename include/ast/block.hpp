//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <memory>
#include <vector>

#include "node.hpp"
#include "token/source_pos.hpp"

namespace xylo::ast
{
struct Stmt;

/*
 * { Stmt Stmt ... Stmt }
 */
struct Block : Node
{
	std::vector<std::unique_ptr<Stmt>> stmts;

	Block(std::vector<std::unique_ptr<Stmt>> stmts, SourcePos pos)
		: Node(pos), stmts(std::move(stmts))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};
}

#endif //_BLOCK_HPP_
