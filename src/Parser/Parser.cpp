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

}

std::unique_ptr<node::Func> Parser::ParseFunc()
{
}

std::unique_ptr<node::Block> Parser::ParseBlock()
{
}

std::unique_ptr<node::Assign> Parser::ParseAssign()
{
}

std::unique_ptr<node::Repeat> Parser::ParseRepeat()
{

}

std::unique_ptr<node::For> Parser::ParseFor()
{
	if (end <= cur)
		return nullptr;

	if (!CompReserved(Reserved::FOR))
	{
		MakeError("expected \"for\"", cur->pos);
		return nullptr;
	}

	auto pos = cur->pos;

	cur++;
	if (end <= cur)
	{
		MakeError("expected ( at the end of source");
		return nullptr;
	}

	if (!CompSymbol(Symbol::LPAREN))
	{
		MakeError("expected (", cur->pos);
		return nullptr;
	}

	cur++;
	auto var_nd = ParseVariable();
	if (var_nd == nullptr)
		return nullptr;

	if (end <= cur)
	{
		MakeError("expected , at the end of source");
		return nullptr;
	}

	if (!CompSymbol(Symbol::COMMA))
	{
		MakeError("expected ,", cur->pos);
		return nullptr;
	}

	cur++;
	auto exp_nd = ParseExp();
	if (exp_nd == nullptr)
		return nullptr;

	if (end <= cur)
	{
		MakeError("expected ) at the end of source");
		return nullptr;
	}

	if (!CompSymbol(Symbol::RPAREN))
	{
		MakeError("expected )", cur->pos);
		return nullptr;
	}

	cur++;
	auto stmt_nd = ParseStmt();
	if (stmt_nd == nullptr)
		return nullptr;

	return std::make_unique<node::For>(node::For{ std::move(var_nd), std::move(exp_nd), std::move(stmt_nd), pos });
}

std::unique_ptr<node::While> Parser::ParseWhile()
{
	if (end <= cur)
		return nullptr;

	if (!CompReserved(Reserved::WHILE))
	{
		MakeError("expected \"while\"", cur->pos);
		return nullptr;
	}

	auto pos = cur->pos;

	cur++;
	if (end <= cur)
	{
		MakeError("expected ( at the end of source");
		return nullptr;
	}

	if (!CompSymbol(Symbol::LPAREN))
	{
		MakeError("expected (", cur->pos);
		return nullptr;
	}

	auto exp_nd = ParseExp();
	if (exp_nd == nullptr)
		return nullptr;

	if (end <= cur)
	{
		MakeError("expected ) at the end of source");
		return nullptr;
	}

	if (!CompSymbol(Symbol::RPAREN))
	{
		MakeError("expected ) at the end of source");
		return nullptr;
	}

	cur++;

	auto stmt_nd = ParseStmt();
	if (stmt_nd == nullptr)
		return nullptr;

	return std::make_unique<node::While>(node::While{ std::move(exp_nd), std::move(stmt_nd), pos });
}

std::unique_ptr<node::Return> Parser::ParseReturn()
{
	if (end <= cur)
		return nullptr;

	if (!CompReserved(Reserved::RETURN))
	{
		MakeError("expected \"return\"", cur->pos);
		return nullptr;
	}

	auto pos = cur->pos;

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

			if (end <= cur)
			{
				MakeError("expected ( at the end of source");
				return nullptr;
			}

			if (CompSymbol(Symbol::RPAREN))
			{
				MakeError("expected (", cur->pos);
				return nullptr;
			}
			cur++;

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

	return std::make_unique<node::Variable>(node::Variable{ cur->GetString(), cur->pos });

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

void Parser::MakeError(const char* msg)
{
	printf("%s\n", msg);
}

void Parser::MakeError(const char* msg, SourcePos pos)
{
	printf("%s [%llu:%llu]\n", msg, pos.line, pos.col);
}
