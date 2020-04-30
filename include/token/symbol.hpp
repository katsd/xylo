//
// Created by Katsu Matsuda on 2020/04/17.
//

#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

namespace xylo
{
enum Symbol
{
	PLUS, // +
	MINUS, // -
	MUL, // *
	DIV, // /
	MOD, // %
	BIN_AND, //&
	BIN_OR, // |
	BIN_XOR, // ^
	BIN_NOT, // ~
	AND, // &&
	OR, // ||
	NOT, // !
	EQUAL, // ==
	NOT_EQUAL, // !=
	GREATER, // >
	GREATER_EQ, // >=
	LESS, // <
	LESS_EQ, // <=
	ASSIGN, // =
	QUE, // ?
	COMMA, // ,
	DOT, // .
	LPAREN, // (
	RPAREN, // )
	LBRACKET, // {
	RBRACKET, // }
};
}

#endif //_SYMBOL_HPP_
