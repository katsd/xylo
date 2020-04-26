//
// Created by Katsu Matsuda on 2020/04/21.
//

#ifndef _INST_HPP_
#define _INST_HPP_

namespace xylo::vm
{
enum Inst
{
	// Push $0
	PUSH,

	// Push A
	PUSH_TOP,

	// Push obj at $0
	PUSH_OBJ,

	// Push global obj at $0
	PUSH_GLOBAL_OBJ,

	// Push const value at $0
	PUSH_CONST,

	// Push 0
	PUSH_ZERO,

	// Push 1
	PUSH_ONE,

	// Push START
	PUSH_START,

	// Set obj at $0 to A
	SET_OBJ,

	// Set global obj at $0 to A
	SET_GLOBAL_OBJ,

	// Set obj at %0 to 0
	SET_OBJ_ZERO,

	// Set obj at %0 to 1
	SET_OBJ_ONE,

	// obj at $0 += 1
	ICR_OBJ,

	// obj at $0 -=1
	DCR_OBJ,

	// Pop stack while A != START
	POP_TO_START,

	// Input value
	// -> Push value
	INPUT,

	// Print A
	OUT,

	// Print "Yay\n"
	YAY,

	// Set pc to $0
	JUMP,

	// Set pc to A
	JUMP_TO_STACK_VALUE,

	// if A != 0 -> Set pc to $0
	JUMP_IF,

	// Push obj_idx_offset
	PUSH_OBJ_IDX_OFFSET,

	// obj_idx_offset += $0
	ADD_OBJ_IDX_OFFSET,

	// obj_idx_offset = A
	SET_OBJ_IDX_OFFSET,

	// func_level++
	ICR_FUNC_LEVEL,

	// func_level--
	DCR_FUNC_LEVEL,

	PUT_RETURN_VALUE,

	PUSH_RETURN_VALUE,

	END,

	// ##################
	// Binary Operators
	// Push A $0 B
	// ##################
	BOPE,

	// A + B
	ADD,

	// A - B
	SUB,

	// A * B
	MUL,

	// A / B
	DIV,

	// A % B
	MOD,

	// A & B
	BIN_AND,

	// A | B
	BIN_OR,

	// A ^ B
	BIN_XOR,

	// A == B ? 1 : 0
	EQUAL,

	// A != B ? 1 : 0
	NOT_EQUAL,

	// A > B ? 1 : 0
	GREATER,

	// A >= B ? 1 : 0
	GREATER_EQ,

	// A < B ? 1 : 0
	LESS,

	// A <= B ? 1 : 0
	LESS_EQ,

	// (A != 0) && (B != 0) ? 1 : 0
	AND,

	// (A != 0) || (B != 0) ? 1 : 0
	OR,

	// ##################
	// Unary Operators
	// ##################

	// 0 - A
	MINUS,

	// A != 0 : 1 : 0
	NOT,

	// ~A
	BIN_NOT,

	DO_NOTHING,
};
}

#endif //_INST_HPP_
