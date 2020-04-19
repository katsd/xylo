//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "Token/Token.hpp"

using namespace xylo;

std::string Token::Symbol2Str(Symbol symbol)
{
	switch (symbol)
	{
	case Symbol::PLUS:
		return "+";
	case Symbol::MINUS:
		return "-";
	case Symbol::MUL:
		return "*";
	case Symbol::DIV:
		return "/";
	case Symbol::MOD:
		return "%";
	case Symbol::BIN_AND:
		return "&";
	case Symbol::BIN_OR:
		return "|";
	case Symbol::BIN_XOR:
		return "^";
	case Symbol::BIN_NOT:
		return "~";
	case Symbol::AND:
		return "&&";
	case Symbol::OR:
		return "||";
	case Symbol::NOT:
		return "!";
	case Symbol::EQUAL:
		return "==";
	case Symbol::NOT_EQUAL:
		return "!=";
	case Symbol::GREATER:
		return ">";
	case Symbol::GREATER_EQ:
		return ">=";
	case Symbol::LESS:
		return "<";
	case Symbol::LESS_EQ:
		return "<=";
	case Symbol::ASSIGN:
		return "=";
	case Symbol::QUE:
		return "?";
	case Symbol::COMMA:
		return ",";
	case Symbol::DOT:
		return ".";
	case Symbol::LPAREN:
		return "(";
	case Symbol::RPAREN:
		return ")";
	case Symbol::LBRACKET:
		return "{";
	case Symbol::RBRACKET:
		return "}";
	default:
		return "";
	}
}

std::string Token::Reserved2Str(Reserved reserved)
{
	switch (reserved)
	{
	case Reserved::FUNC:
		return "func";
	case Reserved::RETURN:
		return "return";
	case Reserved::REPEAT:
		return "repeat";
	case Reserved::FOR:
		return "for";
	case Reserved::WHILE:
		return "while";
	case Reserved::IF:
		return "if";
	case Reserved::ELSE:
		return "else";
	default:
		return "";
	}
}