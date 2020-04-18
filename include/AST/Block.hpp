//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <vector>

#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Stmt;

struct Block
{
	std::vector<Stmt> stmts;

	SourcePos pos;
};
}

#endif //_BLOCK_HPP_
