//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <memory>
#include <vector>

#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Stmt;

/*
 * { Stmt Stmt ... Stmt }
 */
struct Block
{
	std::vector<std::unique_ptr<Stmt>> stmts;

	SourcePos pos;
};
}

#endif //_BLOCK_HPP_
