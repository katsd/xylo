//
// Created by Katsu Matsuda on 2020/04/17.
//

#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <memory>
#include <string>

#include "Token/Token.hpp"
#include "AST/AST.hpp"

namespace xylo
{
class Parser
{
 private:

	const std::string source;

	Token* cur = nullptr;

	Token* end = nullptr;

	std::unique_ptr<node::Stmt> ParseStmt();

	std::unique_ptr<node::FuncDef> ParseFuncDef();

	std::unique_ptr<node::Func> ParseFunc();

	std::unique_ptr<node::Block> ParseBlock();

	std::unique_ptr<node::Assign> ParseAssign();

	std::unique_ptr<node::Repeat> ParseRepeat();

	std::unique_ptr<node::For> ParseFor();

	std::unique_ptr<node::While> ParseWhile();

	std::unique_ptr<node::Return> ParseReturn();

	std::unique_ptr<node::Exp> ParseExp();

	std::unique_ptr<node::BOperator> ParseBOperator();

	std::unique_ptr<node::UOperator> ParseUOperator();

	std::unique_ptr<node::Value> ParseValue();

	std::unique_ptr<node::Int> ParseInt();

	std::unique_ptr<node::Float> ParseFloat();

	std::unique_ptr<node::String> ParseString();

 public:
	explicit Parser(std::string source);

	std::unique_ptr<node::Root> Parse();
};
}

#endif //_PARSER_HPP_
