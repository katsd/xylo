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

	const std::vector<std::set<ast::BOperatorType>> operators = {
		{ ast::BOperatorType::OR },

		{ ast::BOperatorType::AND },

		{ ast::BOperatorType::BIN_OR },

		{ ast::BOperatorType::BIN_XOR },

		{ ast::BOperatorType::BIN_AND },

		{ ast::BOperatorType::EQUAL,
		  ast::BOperatorType::NOT_EQUAL },

		{ ast::BOperatorType::LESS,
		  ast::BOperatorType::LESS_EQ,
		  ast::BOperatorType::GREATER,
		  ast::BOperatorType::GREATER_EQ },

		{ ast::BOperatorType::ADD, ast::BOperatorType::SUB },

		{ ast::BOperatorType::MUL,
		  ast::BOperatorType::DIV,
		  ast::BOperatorType::MOD }
	};

	const std::string source;

	// current node
	Token* cur = nullptr;

	// last node
	Token* end = nullptr;

	std::unique_ptr<ast::Root> ParseRoot();

	std::unique_ptr<ast::Stmt> ParseStmt();

	std::unique_ptr<ast::FuncDef> ParseFuncDef();

	std::unique_ptr<ast::Func> ParseFunc();

	std::unique_ptr<ast::Block> ParseBlock();

	std::unique_ptr<ast::Assign> ParseAssign();

	std::unique_ptr<ast::If> ParseIf();

	std::unique_ptr<ast::Repeat> ParseRepeat();

	std::unique_ptr<ast::For> ParseFor();

	std::unique_ptr<ast::While> ParseWhile();

	std::unique_ptr<ast::Return> ParseReturn();

	std::unique_ptr<ast::Exp> ParseExp();

	std::unique_ptr<ast::Exp> ParseExp(uint8_t ope_rank);

	std::unique_ptr<ast::Exp> ParseExpTail(uint8_t ope_rank, std::unique_ptr<ast::Exp> left);

	// node::Func, node::UOperator, node::Value, (node::Exp)
	std::unique_ptr<ast::Exp> ParseFactor();

	std::unique_ptr<ast::UOperator> ParseUOperator();

	std::unique_ptr<ast::Value> ParseValue();

	std::unique_ptr<ast::Variable> ParseVariable();

	std::unique_ptr<ast::Int> ParseInt();

	std::unique_ptr<ast::Float> ParseFloat();

	std::unique_ptr<ast::String> ParseString();

	// result, func id
	std::tuple<bool, uint64_t> ParseNativeFuncAttribute();

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

	inline bool CompAttribute(Attribute attribute)
	{
		return CompAttribute(cur, attribute);
	}

	static inline bool CompAttribute(Token* token, Attribute attribute)
	{
		return token->type == TokenType::ATTRIBUTE && token->GetAttribute() == attribute;
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

	std::unique_ptr<ast::Root> Parse();
};
}

#endif //_PARSER_HPP_
