//
// Created by Katsu Matsuda on 2020/04/20.
//

#ifndef _IF_HPP_
#define _IF_HPP_

#include <memory>

#include "Exp.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
/*
 * if Exp Stmt
 * if Exp Stmt else Stmt
 */
struct If
{
	std::unique_ptr<Exp> exp;

	std::unique_ptr<Stmt> stmt;

	std::unique_ptr<Stmt> stmt_else;

	SourcePos pos;
};
}

#endif //_IF_HPP_
