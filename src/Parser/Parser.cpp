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

	if (lex_result.tokens.empty())
		return std::make_unique<node::Root>(node::Root{});

	cur = &lex_result.tokens[0];
	end = &lex_result.tokens[lex_result.tokens.size() - 1];

	auto root_nd = ParseRoot();
	if (root_nd == nullptr)
	{
		printf("failed to parse\n");
		return nullptr;
	}

	return root_nd;
}

std::unique_ptr<node::Root> Parser::ParseRoot()
{
	std::vector<node::Root::RootStmt> stmts;

	while (cur <= end)
	{
		if (cur->type == TokenType::RESERVED && cur->GetReserved() == Reserved::FUNC)
		{
			auto nd = ParseFuncDef();
			if (nd == nullptr)
				return nullptr;
			stmts.emplace_back(std::move(nd));
			continue;
		}

		auto nd = ParseStmt();
		if (nd == nullptr)
			return nullptr;
		stmts.emplace_back(std::move(nd));
	}

	return std::make_unique<node::Root>(node::Root{ std::move(stmts), SourcePos{}});
}

std::unique_ptr<node::Stmt> Parser::ParseStmt()
{
	auto pos = cur->pos;

	// Block
	if (cur->type == TokenType::SYMBOL && cur->GetSymbol() == Symbol::LBRACKET)
	{
		auto nd = ParseBlock();
		if (nd == nullptr)
			return nullptr;
		return std::make_unique<node::Stmt>(node::Stmt{ std::move(nd), pos });
	}

	if (cur->type == TokenType::IDENTIFIER)
	{
		// ASSIGN
		if (cur + 1 <= end && CompSymbol(cur + 1, Symbol::ASSIGN))
		{
			auto nd = ParseAssign();
			if (nd == nullptr)
				return nullptr;
			return std::make_unique<node::Stmt>(node::Stmt{ std::move(nd), pos });
		}

		// Func
		auto nd = ParseFunc();
		if (nd == nullptr)
			return nullptr;
		return std::make_unique<node::Stmt>(node::Stmt{ std::move(nd), pos });
	}

	if (cur->type != TokenType::RESERVED)
	{
		MakeError("expected statement", cur->pos);
		return nullptr;
	}

	node::Stmt::StmtType stmt;

	switch (cur->GetReserved())
	{
	case Reserved::IF:
	{
		auto nd = ParseIf();
		if (nd == nullptr)
			return nullptr;
		stmt = std::move(nd);
	}
		break;

	case Reserved::WHILE:
	{
		auto nd = ParseWhile();
		if (nd == nullptr)
			return nullptr;
		stmt = std::move(nd);
	}
		break;

	case Reserved::REPEAT:
	{
		auto nd = ParseRepeat();
		if (nd == nullptr)
			return nullptr;
		stmt = std::move(nd);
	}
		break;

	case Reserved::FOR:
	{
		auto nd = ParseFor();
		if (nd == nullptr)
			return nullptr;
		stmt = std::move(nd);
	}
		break;

	case Reserved::RETURN:
	{
		auto nd = ParseReturn();
		if (nd == nullptr)
			return nullptr;
		stmt = std::move(nd);
	}
		break;

	default:
		return nullptr;
	}

	return std::make_unique<node::Stmt>(node::Stmt{ std::move(stmt), pos });
}

std::unique_ptr<node::FuncDef> Parser::ParseFuncDef()
{
	if (end < cur)
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
			break;

		auto arg_nd = ParseVariable();
		if (arg_nd == nullptr)
			return nullptr;
		args.emplace_back(std::move(arg_nd));

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
	if (end < cur)
		return nullptr;

	if (cur->type != TokenType::IDENTIFIER)
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
		args.emplace_back(std::move(arg_nd));

		if (CheckSymbol(Symbol::RPAREN, false))
			break;

		if (!CheckSymbol(Symbol::COMMA))
			return nullptr;
	}

	return std::make_unique<node::Func>(node::Func{ func_name, std::move(args), pos });
}

std::unique_ptr<node::Block> Parser::ParseBlock()
{
	if (end < cur)
		return nullptr;

	auto pos = cur->pos;

	if (!CheckSymbol(Symbol::LBRACKET))
		return nullptr;

	std::vector<std::unique_ptr<node::Stmt>> stmts;

	while (true)
	{
		if (CheckSymbol(Symbol::RBRACKET, false))
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
	if (end < cur)
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

std::unique_ptr<node::If> Parser::ParseIf()
{
	if (end < cur)
		return nullptr;

	auto pos = cur->pos;

	if (!CheckReserved(Reserved::IF))
		return nullptr;

	auto exp_nd = ParseExp();
	if (exp_nd == nullptr)
		return nullptr;

	auto stmt_nd = ParseStmt();
	if (stmt_nd == nullptr)
		return nullptr;

	std::unique_ptr<node::Stmt> stmt_else_nd = nullptr;

	if (CheckReserved(Reserved::ELSE, false))
	{
		stmt_else_nd = ParseStmt();
		if (stmt_else_nd == nullptr)
			return nullptr;
	}

	return std::make_unique<node::If>(node::If{ std::move(exp_nd), std::move(stmt_nd), std::move(stmt_else_nd), pos });
}

std::unique_ptr<node::Repeat> Parser::ParseRepeat()
{
	if (end < cur)
		return nullptr;

	auto pos = cur->pos;

	if (!CheckReserved(Reserved::REPEAT))
		return nullptr;

	auto exp_nd = ParseExp();
	if (exp_nd == nullptr)
		return nullptr;

	auto stmt_nd = ParseStmt();
	if (stmt_nd == nullptr)
		return nullptr;

	return std::make_unique<node::Repeat>(node::Repeat{ std::move(exp_nd), std::move(stmt_nd), pos });
}

std::unique_ptr<node::For> Parser::ParseFor()
{
	if (end < cur)
		return nullptr;

	auto pos = cur->pos;

	if (!CheckReserved(Reserved::FOR))
		return nullptr;

	auto var_nd = ParseVariable();
	if (var_nd == nullptr)
		return nullptr;

	if (!CheckSymbol(Symbol::COMMA))
		return nullptr;

	auto exp_nd = ParseExp();
	if (exp_nd == nullptr)
		return nullptr;

	auto stmt_nd = ParseStmt();
	if (stmt_nd == nullptr)
		return nullptr;

	return std::make_unique<node::For>(node::For{ std::move(var_nd), std::move(exp_nd), std::move(stmt_nd), pos });
}

std::unique_ptr<node::While> Parser::ParseWhile()
{
	if (end < cur)
		return nullptr;

	auto pos = cur->pos;

	if (!CheckReserved(Reserved::WHILE))
		return nullptr;

	auto exp_nd = ParseExp();
	if (exp_nd == nullptr)
		return nullptr;

	auto stmt_nd = ParseStmt();
	if (stmt_nd == nullptr)
		return nullptr;

	return std::make_unique<node::While>(node::While{ std::move(exp_nd), std::move(stmt_nd), pos });
}

std::unique_ptr<node::Return> Parser::ParseReturn()
{
	if (end < cur)
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
	if (end < cur || cur->type != TokenType::SYMBOL)
		return left;

	auto pos = cur->pos;

	auto opes = operators[ope_rank];
	auto ope = node::BOperator::Symbol2BOperator(cur->GetSymbol());
	if (!opes.count(ope))
		return left;

	auto ope_pos = cur->pos;

	cur++;

	auto right = ParseExp(ope_rank + 1);

	std::unique_ptr<node::BOperator> nd_bope(new node::BOperator{ ope, std::move(left), std::move(right), ope_pos });

	std::unique_ptr<node::Exp> nd_exp(new node::Exp{ std::move(nd_bope), pos });

	return ParseExpTail(ope_rank, std::move(nd_exp));
}

std::unique_ptr<node::Exp> Parser::ParseFactor()
{
	if (end < cur)
		return nullptr;

	auto pos = cur->pos;

	if (cur->type == TokenType::SYMBOL)
	{
		if (CompSymbol(Symbol::LPAREN))
		{
			cur++;

			auto nd = ParseExp();

			if (!CheckSymbol(Symbol::RPAREN))
				return nullptr;

			return nd;
		}

		auto nd = ParseUOperator();
		if (nd == nullptr)
			return nullptr;
		return std::make_unique<node::Exp>(node::Exp{ std::move(nd), pos });
	}

	if (cur->type == TokenType::IDENTIFIER &&
		(cur + 1) <= end &&
		CompSymbol(cur + 1, Symbol::LPAREN))
	{
		auto nd = ParseFunc();
		if (nd == nullptr)
			return nullptr;
		return std::make_unique<node::Exp>(node::Exp{ std::move(nd), pos });
	}

	auto nd = ParseValue();
	if (nd == nullptr)
		return nullptr;
	return std::make_unique<node::Exp>(node::Exp{ std::move(nd), pos });
}

std::unique_ptr<node::UOperator> Parser::ParseUOperator()
{
	if (end < cur)
		return nullptr;

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

	case Symbol::BIN_NOT:
		ope = node::UOperatorType::BIN_NOT;
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
	if (end < cur)
		return nullptr;

	auto pos = cur->pos;

	switch (cur->type)
	{
	case TokenType::INT:
	{
		auto res = ParseInt();
		if (res == nullptr) return nullptr;
		return std::make_unique<node::Value>(node::Value{ std::move(res), pos });
	}
	case TokenType::FLOAT:
	{
		auto res = ParseFloat();
		if (res == nullptr) return nullptr;
		return std::make_unique<node::Value>(node::Value{ std::move(res), pos });
	}
	case TokenType::STRING:
	{
		auto res = ParseString();
		if (res == nullptr) return nullptr;
		return std::make_unique<node::Value>(node::Value{ std::move(res), pos });
	}
	case TokenType::IDENTIFIER:
	{
		auto res = ParseVariable();
		if (res == nullptr) return nullptr;
		return std::make_unique<node::Value>(node::Value{ std::move(res), pos });
	}
	default:
		return nullptr;
	}
}

std::unique_ptr<node::Variable> Parser::ParseVariable()
{
	std::string identifier;
	if (!CheckIdentifier(identifier))
		return nullptr;

	auto pos = (cur - 1)->pos;

	return std::make_unique<node::Variable>(node::Variable{ identifier, pos });
}

std::unique_ptr<node::Int> Parser::ParseInt()
{
	if (end < cur)
		return nullptr;

	if (cur->type != TokenType::INT)
		return nullptr;

	auto pos = cur->pos;
	uint64_t value = cur->GetInt();
	cur++;

	return std::make_unique<node::Int>(node::Int{ value, pos });
}

std::unique_ptr<node::Float> Parser::ParseFloat()
{
	if (end < cur)
		return nullptr;

	if (cur->type != TokenType::FLOAT)
		return nullptr;

	auto pos = cur->pos;
	double value = cur->GetFloat();
	cur++;

	return std::make_unique<node::Float>(node::Float{ value, pos });
}

std::unique_ptr<node::String> Parser::ParseString()
{
	if (end < cur)
		return nullptr;

	if (cur->type != TokenType::STRING)
		return nullptr;

	auto pos = cur->pos;
	std::string value = cur->GetString();
	cur++;

	return std::make_unique<node::String>(node::String{ value, pos });
}

bool Parser::CheckSymbol(Symbol symbol, bool out_error)
{
	if (end < cur)
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
	if (end < cur)
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
	if (end < cur)
	{
		if (out_error)
			MakeError("expected an identifier at the end of source");
		return false;
	}

	if (cur->type != TokenType::IDENTIFIER)
	{
		if (out_error)
			MakeError("expected an identifier", cur->pos);
		return false;
	}

	identifier = cur->GetIdentifier();
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
