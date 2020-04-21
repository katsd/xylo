//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "CodeGen/CodeGen.hpp"

using namespace xylo;

std::vector<uint64_t> CodeGen::GenerateCode()
{
	code.clear();
	InitConstTable();

	return code;
}

bool CodeGen::ConvertRoot(std::unique_ptr<node::Root>& node, uint64_t scope_id)
{

}

bool CodeGen::ConvertStmt(std::unique_ptr<node::Stmt>& node, uint64_t scope_id)
{

}

bool CodeGen::ConvertFuncDef(std::unique_ptr<node::FuncDef>& node, uint64_t scope_id)
{

}

bool CodeGen::ConvertFunc(std::unique_ptr<node::Func>& node, uint64_t scope_id)
{

}

bool CodeGen::ConvertBlock(std::unique_ptr<node::Block>& node, uint64_t scope_id)
{

}

bool CodeGen::ConvertAssign(std::unique_ptr<node::Assign>& node, uint64_t scope_id)
{

}

bool CodeGen::ConvertIf(std::unique_ptr<node::If>& node, uint64_t scope_id)
{

}

bool CodeGen::ConvertRepeat(std::unique_ptr<node::Repeat>& node, uint64_t scope_id)
{

}

bool CodeGen::ConvertFor(std::unique_ptr<node::For>& node, uint64_t scope_id)
{

}

bool CodeGen::ConvertWhile(std::unique_ptr<node::While>& node, uint64_t scope_id)
{

}

bool CodeGen::ConvertReturn(std::unique_ptr<node::Return>& node, uint64_t scope_id)
{

}

bool CodeGen::ConvertExp(std::unique_ptr<node::Exp>& node, uint64_t scope_id)
{
	if (std::holds_alternative<std::unique_ptr<node::Value>>(node->value))
		return ConvertValue(std::get<std::unique_ptr<node::Value>>(node->value), scope_id);

	if (std::holds_alternative<std::unique_ptr<node::BOperator>>(node->value))
		return ConvertBOperator(std::get<std::unique_ptr<node::BOperator>>(node->value), scope_id);

	if (std::holds_alternative<std::unique_ptr<node::UOperator>>(node->value))
		return ConvertUOperator(std::get<std::unique_ptr<node::UOperator>>(node->value), scope_id);

	if (std::holds_alternative<std::unique_ptr<node::Func>>(node->value))
		return ConvertFunc(std::get<std::unique_ptr<node::Func>>(node->value), scope_id);

	return MakeError("unknown expression", *node);
}

bool CodeGen::ConvertBOperator(std::unique_ptr<node::BOperator>& node, uint64_t scope_id)
{
	if (!ConvertExp(node->right, scope_id))
		return false;

	if (!ConvertExp(node->left, scope_id))
		return false;

	switch (node->type)
	{
	case node::BOperatorType::OR:
		code.push_back(vm::Inst::OR);
		break;
	case node::BOperatorType::AND:
		code.push_back(vm::Inst::AND);
		break;
	case node::BOperatorType::BIN_OR:
		code.push_back(vm::Inst::BIN_OR);
		break;
	case node::BOperatorType::BIN_XOR:
		code.push_back(vm::Inst::BIN_XOR);
		break;
	case node::BOperatorType::BIN_AND:
		code.push_back(vm::Inst::BIN_AND);
		break;
	case node::BOperatorType::EQUAL:
		code.push_back(vm::Inst::EQUAL);
		break;
	case node::BOperatorType::NOT_EQUAL:
		code.push_back(vm::Inst::NOT_EQUAL);
		break;
	case node::BOperatorType::LESS:
		code.push_back(vm::Inst::LESS);
		break;
	case node::BOperatorType::LESS_EQ:
		code.push_back(vm::Inst::LESS_EQ);
		break;
	case node::BOperatorType::GREATER:
		code.push_back(vm::Inst::GREATER);
		break;
	case node::BOperatorType::GREATER_EQ:
		code.push_back(vm::Inst::GREATER_EQ);
		break;
	case node::BOperatorType::ADD:
		code.push_back(vm::Inst::ADD);
		break;
	case node::BOperatorType::SUB:
		code.push_back(vm::Inst::SUB);
		break;
	case node::BOperatorType::MUL:
		code.push_back(vm::Inst::MUL);
		break;
	case node::BOperatorType::DIV:
		code.push_back(vm::Inst::DIV);
		break;
	case node::BOperatorType::MOD:
		code.push_back(vm::Inst::MOD);
		break;
	default:
		return MakeError("unknown operator", *node);
	}

	return true;
}

bool CodeGen::ConvertUOperator(std::unique_ptr<node::UOperator>& node, uint64_t scope_id)
{
	if (!ConvertExp(node->exp, scope_id))
		return false;

	switch (node->type)
	{
	case node::UOperatorType::MINUS:
		code.push_back(vm::Inst::MINUS);
		break;
	case node::UOperatorType::NOT:
		code.push_back(vm::Inst::NOT);
	case node::UOperatorType::BIN_NOT:
		code.push_back(vm::Inst::BIN_NOT);
	}

	return true;
}

bool CodeGen::ConvertValue(std::unique_ptr<node::Value>& node, uint64_t scope_id)
{
	if (std::holds_alternative<std::unique_ptr<node::Int>>(node->value))
		return ConvertInt(std::get<std::unique_ptr<node::Int>>(node->value));

	if (std::holds_alternative<std::unique_ptr<node::Float>>(node->value))
		return ConvertFloat(std::get<std::unique_ptr<node::Float>>(node->value));

	if (std::holds_alternative<std::unique_ptr<node::String>>(node->value))
		return ConvertString(std::get<std::unique_ptr<node::String>>(node->value));

	if (std::holds_alternative<std::unique_ptr<node::Variable>>(node->value))
		return ConvertVariable(std::get<std::unique_ptr<node::Variable>>(node->value), scope_id);

	return MakeError("unknown value", *node);
}

bool CodeGen::ConvertVariable(std::unique_ptr<node::Variable>& node, uint64_t scope_id)
{

}

bool CodeGen::ConvertInt(std::unique_ptr<node::Int>& node)
{
	PushConst(vm::Obj{ node->value });
	return true;
}

bool CodeGen::ConvertFloat(std::unique_ptr<node::Float>& node)
{
	PushConst(vm::Obj{ node->value });
	return true;
}

bool CodeGen::ConvertString(std::unique_ptr<node::String>& node)
{
	PushConst(vm::Obj{ node->value });
	return true;
}

void CodeGen::InitConstTable()
{
	const_table.clear();
	const_address.clear();
}

uint64_t CodeGen::AddConst(vm::Obj& obj)
{
	if (const_address.find(obj) == const_address.end())
	{
		const_address[obj] = const_table.size();
		const_table.push_back(obj);
	}

	return const_address[obj];
}

bool CodeGen::MakeError(const char* msg, const node::Node& node)
{
	printf("%s [%llu,%llu]\n", msg, node.pos.line, node.pos.col);

	return false;
}