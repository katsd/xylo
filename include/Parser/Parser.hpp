//
// Created by Katsu Matsuda on 2020/04/17.
//

#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <memory>
#include <vector>
#include <string>
#include <set>

#include "Token/Token.hpp"
#include "AST/AST.hpp"

namespace xylo
{
class Parser
{
 private:

	const std::vector<std::set<node::BOperatorType>> operators = {
		{ node::BOperatorType::OR },

		{ node::BOperatorType::AND },

		{ node::BOperatorType::BIN_OR },

		{ node::BOperatorType::BIN_XOR },

		{ node::BOperatorType::BIN_AND },

		{ node::BOperatorType::EQUAL,
		  node::BOperatorType::NOT_EQUAL },

		{ node::BOperatorType::LESS,
		  node::BOperatorType::LESS_EQ,
		  node::BOperatorType::GREATER,
		  node::BOperatorType::GREATER_EQ },

		{ node::BOperatorType::ADD, node::BOperatorType::SUB },

		{ node::BOperatorType::MUL,
		  node::BOperatorType::DIV,
		  node::BOperatorType::MOD }
	};

	const std::string source;

	// current node
	Token* cur = nullptr;

	// last node
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

	std::unique_ptr<node::Exp> ParseExp(uint8_t ope_rank);

	std::unique_ptr<node::Exp> ParseExpTail(uint8_t ope_rank, std::unique_ptr<node::Exp> left);

	// node::Func, node::UOperator, node::Value, (node::Exp)
	std::unique_ptr<node::Exp> ParseFactor();

	std::unique_ptr<node::UOperator> ParseUOperator();

	std::unique_ptr<node::Value> ParseValue();

	std::unique_ptr<node::Variable> ParseVariable();

	std::unique_ptr<node::Int> ParseInt();

	std::unique_ptr<node::Float> ParseFloat();

	std::unique_ptr<node::String> ParseString();

	bool CheckSymbol(Symbol symbol, bool out_error = true);

	bool CheckReserved(Reserved reserved, bool out_error = true);

	bool CheckIdentifier(std::string& identifier, bool out_error = true);

	inline bool CompSymbol(Symbol symbol)
	{
		return CompSymbol(cur, symbol);
	}

	static inline bool CompSymbol(Token* token, Symbol symbol)
	{
		return token->type == TokenType::SYMBOL && token->GetSymbol() == symbol;
	}

	inline bool CompReserved(Reserved reserved)
	{
		return CompReserved(cur, reserved);
	}

	static inline bool CompReserved(Token* token, Reserved reserved)
	{
		return token->type == TokenType::RESERVED && token->GetReserved() == reserved;
	}

	static void MakeError(const char* msg);

	static void MakeError(const char* msg, SourcePos pos);

 public:
	explicit Parser(std::string source);

	std::unique_ptr<node::Root> Parse();
};
}

#endif //_PARSER_HPP_
