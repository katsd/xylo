//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _STMT_HPP_
#define _STMT_HPP_

#include <variant>
#include <memory>

#include "Block.hpp"
#include "Assign.hpp"
#include "Func.hpp"
#include "While.hpp"
#include "Repeat.hpp"
#include "For.hpp"
#include "If.hpp"
#include "Return.hpp"

namespace xylo::node
{

struct Stmt
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

	StmtType value;
};

}

#endif //_STMT_HPP_
