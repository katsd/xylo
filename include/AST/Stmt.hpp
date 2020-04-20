//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _STMT_HPP_
#define _STMT_HPP_

#include <variant>
#include <memory>

#include "Node.hpp"
#include "Block.hpp"
#include "Assign.hpp"
#include "Func.hpp"
#include "While.hpp"
#include "Repeat.hpp"
#include "For.hpp"
#include "If.hpp"
#include "Return.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
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
		: stmt(std::move(stmt)), Node(pos)
	{
	}

	std::string Node2Str(uint64_t indent) override;
};

}

#endif //_STMT_HPP_
