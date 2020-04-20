//
// Created by Katsu Matsuda on 2020/04/18.
//

#include "Parser/Parser.hpp"

#include <memory>
#include "Parser/Lexer.hpp"

using namespace xylo;

Parser::Parser(std::string source)
	: source(std::move(source))
{
}

std::unique_ptr<node::Root> Parser::Parse()
{
	auto lex_result = Lexer(source).Tokenize();

	if (!lex_result.success)
	{
		printf("failed to parse\n");
		return nullptr;
	}

	cur = &lex_result.tokens[0];

	end = &lex_result.tokens[lex_result.tokens.size() - 1];

	return nullptr;
}

std::unique_ptr<node::Stmt> Parser::ParseStmt()
{

}

std::unique_ptr<node::FuncDef> Parser::ParseFuncDef()
{
	if (end <= cur)
		return nullptr;

	auto pos = cur->pos;

	if (!CheckReserved(Reserved::FUNC))
		return nullptr;

	std::string func_name;
	if (!CheckIdentifier(func_name))
		return nullptr;

	if (!CheckSymbol(Symbol::LPAREN))
		return nullptr;

	std::vector<std::unique_ptr<node::Variable>> args;

	while (true)
	{
		if (CheckSymbol(Symbol::RPAREN, false))
			break;;

		auto arg_nd = ParseVariable();
		if (arg_nd == nullptr)
			return nullptr;

		if (CheckSymbol(Symbol::RPAREN, false))
			break;

		if (!CheckSymbol(Symbol::COMMA))
			return nullptr;
	}

	auto stmt_nd = ParseStmt();
	if (stmt_nd == nullptr)
		return nullptr;

	return std::make_unique<node::FuncDef>(node::FuncDef{ func_name, std::move(args), std::move(stmt_nd), pos });
}

std::unique_ptr<node::Func> Parser::ParseFunc()
{
	if (end <= cur)
		return nullptr;

	if (cur->type != TokenType::NAME)
		return nullptr;

	auto pos = cur->pos;

	auto func_name = cur->GetString();
	cur++;

	if (!CheckSymbol(Symbol::LPAREN))
		return nullptr;

	std::vector<std::unique_ptr<node::Exp>> args;

	while (true)
	{
		if (CheckSymbol(Symbol::RPAREN, false))
			break;

		auto arg_nd = ParseExp();
		if (arg_nd == nullptr)
			return nullptr;

		if (CheckSymbol(Symbol::RPAREN, false))
			break;

		if (!CheckSymbol(Symbol::COMMA))
			return nullptr;
	}

	return std::make_unique<node::Func>(node::Func{ func_name, std::move(args), pos });
}

std::unique_ptr<node::Block> Parser::ParseBlock()
{
	if (end <= cur)
		return nullptr;

	auto pos = cur->pos;

	if (!CheckSymbol(Symbol::LBRACKET))
		return nullptr;

	std::vector<std::unique_ptr<node::Stmt>> stmts;

	while (true)
	{
		if (CheckSymbol(Symbol::RBRACKET))
			break;

		auto stmt_nd = ParseStmt();
		if (stmt_nd == nullptr)
			return nullptr;

		stmts.push_back(std::move(stmt_nd));
	}

	return std::make_unique<node::Block>(node::Block{ std::move(stmts), pos });
}

std::unique_ptr<node::Assign> Parser::ParseAssign()
{
	if (end <= cur)
		return nullptr;

	auto pos = cur->pos;

	auto var_nd = ParseVariable();
	if (var_nd == nullptr)
		return nullptr;

	if (!CheckSymbol(Symbol::ASSIGN))
		return nullptr;

	auto exp_nd = ParseExp();
	if (exp_nd == nullptr)
		return nullptr;

	return std::make_unique<node::Assign>(node::Assign{ std::move(var_nd), std::move(exp_nd), pos });
}

std::unique_ptr<node::Repeat> Parser::ParseRepeat()
{
	if (end <= cur)
		return nullptr;

	auto pos = cur->pos;

	if (!CheckReserved(Reserved::REPEAT))
		return nullptr;

	if (!CheckSymbol(Symbol::LPAREN))
		return nullptr;

	auto exp_nd = ParseExp();
	if (exp_nd == nullptr)
		return nullptr;

	if (!CheckSymbol(Symbol::RPAREN))
		return nullptr;

	auto stmt_nd = ParseStmt();
	if (stmt_nd == nullptr)
		return nullptr;

	return std::make_unique<node::Repeat>(node::Repeat{ std::move(exp_nd), std::move(stmt_nd), pos });
}

std::unique_ptr<node::For> Parser::ParseFor()
{
	if (end <= cur)
		return nullptr;

	auto pos = cur->pos;

	if (!CheckReserved(Reserved::FOR))
		return nullptr;

	if (!CheckSymbol(Symbol::LPAREN))
		return nullptr;

	auto var_nd = ParseVariable();
	if (var_nd == nullptr)
		return nullptr;

	if (!CheckSymbol(Symbol::COMMA))
		return nullptr;

	auto exp_nd = ParseExp();
	if (exp_nd == nullptr)
		return nullptr;

	if (!CheckSymbol(Symbol::RPAREN))
		return nullptr;

	auto stmt_nd = ParseStmt();
	if (stmt_nd == nullptr)
		return nullptr;

	return std::make_unique<node::For>(node::For{ std::move(var_nd), std::move(exp_nd), std::move(stmt_nd), pos });
}

std::unique_ptr<node::While> Parser::ParseWhile()
{
	if (end <= cur)
		return nullptr;

	auto pos = cur->pos;

	if (!CheckReserved(Reserved::WHILE))
		return nullptr;

	if (!CheckSymbol(Symbol::LPAREN))
		return nullptr;

	auto exp_nd = ParseExp();
	if (exp_nd == nullptr)
		return nullptr;

	if (!CheckSymbol(Symbol::RPAREN))
		return nullptr;

	auto stmt_nd = ParseStmt();
	if (stmt_nd == nullptr)
		return nullptr;

	return std::make_unique<node::While>(node::While{ std::move(exp_nd), std::move(stmt_nd), pos });
}

std::unique_ptr<node::Return> Parser::ParseReturn()
{
	if (end <= cur)
		return nullptr;

	auto pos = cur->pos;

	if (!CheckReserved(Reserved::RETURN))
		return nullptr;

	auto nd = ParseExp();
	if (nd == nullptr)
		return nullptr;

	return std::make_unique<node::Return>(node::Return{ std::move(nd), pos });
}

std::unique_ptr<node::Exp> Parser::ParseExp()
{
	return ParseExp(0);
}

std::unique_ptr<node::Exp> Parser::ParseExp(uint8_t ope_rank)
{
	if (ope_rank >= operators.size())
		return ParseFactor();

	auto opes = operators[ope_rank];

	auto left = ParseExp(ope_rank + 1);

	if (left == nullptr)
		return nullptr;

	return ParseExpTail(ope_rank, std::move(left));
}

std::unique_ptr<node::Exp> Parser::ParseExpTail(uint8_t ope_rank, std::unique_ptr<node::Exp> left)
{
	if (end <= cur || cur->type != TokenType::SYMBOL)
		return left;

	auto opes = operators[ope_rank];
	auto ope = node::BOperator::Symbol2BOperator(cur->GetSymbol());
	if (!opes.count(ope))
		return left;

	auto ope_pos = cur->pos;

	cur++;

	auto right = ParseExp(ope_rank + 1);

	std::unique_ptr<node::BOperator> nd_bope(new node::BOperator{ ope, std::move(left), std::move(right), ope_pos });

	std::unique_ptr<node::Exp> nd_exp(new node::Exp{ std::move(nd_bope) });

	return ParseExpTail(ope_rank, std::move(nd_exp));
}

std::unique_ptr<node::Exp> Parser::ParseFactor()
{
	if (cur->type == TokenType::SYMBOL)
	{
		if (CompSymbol(Symbol::LPAREN))
		{
			auto factor_pos = cur->pos;
			cur++;

			auto nd = ParseExp();

			if (!CheckSymbol(Symbol::RPAREN))
				return nullptr;

			return nd;
		}

		auto nd = ParseUOperator();
		if (nd == nullptr)
			return nullptr;
		return std::make_unique<node::Exp>(node::Exp{ std::move(nd) });
	}

	if (cur->type == TokenType::NAME &&
		(cur + 1) >= end &&
		CompSymbol(cur + 1, Symbol::LPAREN))
	{
		auto nd = ParseFunc();
		if (nd == nullptr)
			return nullptr;
		return std::make_unique<node::Exp>(node::Exp{ std::move(nd) });
	}

	auto nd = ParseValue();
	if (nd == nullptr)
		return nullptr;
	return std::make_unique<node::Exp>(node::Exp{ std::move(nd) });
}

std::unique_ptr<node::UOperator> Parser::ParseUOperator()
{
	if (cur->type != TokenType::SYMBOL)
		return nullptr;

	auto ope_pos = cur->pos;

	node::UOperatorType ope;

	switch (cur->GetSymbol())
	{
	case Symbol::MINUS:
		ope = node::UOperatorType::MINUS;
		break;

	case Symbol::NOT:
		ope = node::UOperatorType::NOT_EQ;
		break;

	default:
		return nullptr;
	}

	cur++;
	auto nd = ParseFactor();
	if (nd == nullptr)
		return nullptr;
	return std::make_unique<node::UOperator>(node::UOperator{ ope, std::move(nd), ope_pos });
}

std::unique_ptr<node::Value> Parser::ParseValue()
{
	switch (cur->type)
	{
	case TokenType::INT:
	{
		auto res = ParseInt();
		if (res == nullptr) return nullptr;
		return std::make_unique<node::Value>(std::move(res));
	}
	case TokenType::FLOAT:
	{
		auto res = ParseFloat();
		if (res == nullptr) return nullptr;
		return std::make_unique<node::Value>(std::move(res));
	}
	case TokenType::STRING:
	{
		auto res = ParseString();
		if (res == nullptr) return nullptr;
		return std::make_unique<node::Value>(std::move(res));
	}
	case TokenType::NAME:
	{
		auto res = ParseVariable();
		if (res == nullptr) return nullptr;
		return std::make_unique<node::Value>(std::move(res));
	}
	default:
		return nullptr;
	}
}

std::unique_ptr<node::Variable> Parser::ParseVariable()
{
	if (end <= cur)
		return nullptr;

	if (cur->type != TokenType::NAME)
		return nullptr;

	return std::make_unique<node::Variable>(node::Variable{ cur->GetName(), cur->pos });
}

std::unique_ptr<node::Int> Parser::ParseInt()
{
	if (end <= cur)
		return nullptr;

	if (cur->type != TokenType::INT)
		return nullptr;

	return std::make_unique<node::Int>(node::Int{ cur->GetInt(), cur->pos });
}

std::unique_ptr<node::Float> Parser::ParseFloat()
{
	if (end <= cur)
		return nullptr;

	if (cur->type != TokenType::FLOAT)
		return nullptr;

	return std::make_unique<node::Float>(node::Float{ cur->GetFloat(), cur->pos });
}

std::unique_ptr<node::String> Parser::ParseString()
{
	if (cur->type != TokenType::STRING)
		return nullptr;

	return std::make_unique<node::String>(node::String{ cur->GetString(), cur->pos });
}

bool Parser::CheckSymbol(Symbol symbol, bool out_error)
{
	if (cur <= end)
	{
		if (out_error)
			MakeError(("expected " + Token::Symbol2Str(symbol) + " at the end of source").c_str());
		return false;
	}

	if (!CompSymbol(symbol))
	{
		if (out_error)
			MakeError(("expected " + Token::Symbol2Str(symbol)).c_str(), cur->pos);
		return false;
	}

	cur++;
	return true;
}

bool Parser::CheckReserved(Reserved reserved, bool out_error)
{
	if (cur <= end)
	{
		if (out_error)
			MakeError(("expected \"" + Token::Reserved2Str(reserved) + "\" at the end of source").c_str());
		return false;
	}

	if (!CompReserved(reserved))
	{
		if (out_error)
			MakeError(("expected \"" + Token::Reserved2Str(reserved) + "\"").c_str(), cur->pos);
		return false;
	}

	cur++;
	return true;
}

bool Parser::CheckIdentifier(std::string& identifier, bool out_error)
{
	if (cur <= end)
	{
		if (out_error)
			MakeError("expected an identifier at the end of source");
		return false;
	}

	if (cur->type != TokenType::NAME)
	{
		if (out_error)
			MakeError("expected an identifier", cur->pos);
		return false;
	}

	identifier = cur->GetName();
	cur++;
	return true;
}

void Parser::MakeError(const char* msg)
{
	printf("%s\n", msg);
}

void Parser::MakeError(const char* msg, SourcePos pos)
{
	printf("%s [%llu:%llu]\n", msg, pos.line, pos.col);
}
