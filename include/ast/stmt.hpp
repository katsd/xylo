//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _STMT_HPP_
#define _STMT_HPP_

#include <variant>
#include <memory>

#include "node.hpp"
#include "block.hpp"
#include "assign.hpp"
#include "func.hpp"
#include "while.hpp"
#include "repeat.hpp"
#include "for.hpp"
#include "if.hpp"
#include "return.hpp"
#include "token/source_pos.hpp"

namespace xylo::ast
{
struct Stmt : Node
{
	typedef std::variant<
		std::unique_ptr<Block>,
		std::unique_ptr<Assign>,
		std::unique_ptr<Func>,
		std::unique_ptr<If>,
		std::unique_ptr<While>,
		std::unique_ptr<Repeat>,
		std::unique_ptr<For>,
		std::unique_ptr<Return>
	> StmtType;

	StmtType stmt;

	Stmt(StmtType stmt, SourcePos pos)
		: Node(pos), stmt(std::move(stmt))
	{
	}

	std::string Node2Str(uint64_t indent) override;
};

}

#endif //_STMT_HPP_
