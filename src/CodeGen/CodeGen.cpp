//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "CodeGen/CodeGen.hpp"

using namespace xylo;

std::vector<uint64_t> CodeGen::GenerateCode()
{
	code.clear();

	InitVariable();
	InitConstTable();
	InitScope();
	InitFunc();

	return code;
}

bool CodeGen::ConvertRoot(std::unique_ptr<node::Root>& node, uint64_t scope_id)
{

}

bool CodeGen::ConvertStmt(std::unique_ptr<node::Stmt>& node, uint64_t scope_id, bool is_new_scope)
{
	if (is_new_scope)
		scope_id = GetNewScope();

	bool res;

	if (std::holds_alternative<std::unique_ptr<node::Block>>(node->stmt))
	{
		res = ConvertBlock(std::get<std::unique_ptr<node::Block>>(node->stmt), scope_id);
	}
	else if (std::holds_alternative<std::unique_ptr<node::Assign>>(node->stmt))
	{
		res = ConvertAssign(std::get<std::unique_ptr<node::Assign>>(node->stmt), scope_id);
	}
	else if (std::holds_alternative<std::unique_ptr<node::Func>>(node->stmt))
	{
		res = ConvertFunc(std::get<std::unique_ptr<node::Func>>(node->stmt), scope_id);
	}
	else if (std::holds_alternative<std::unique_ptr<node::If>>(node->stmt))
	{
		res = ConvertIf(std::get<std::unique_ptr<node::If>>(node->stmt), scope_id);
	}
	else if (std::holds_alternative<std::unique_ptr<node::While>>(node->stmt))
	{
		res = ConvertWhile(std::get<std::unique_ptr<node::While>>(node->stmt), scope_id);
	}
	else if (std::holds_alternative<std::unique_ptr<node::Repeat>>(node->stmt))
	{
		res = ConvertRepeat(std::get<std::unique_ptr<node::Repeat>>(node->stmt), scope_id);
	}
	else if (std::holds_alternative<std::unique_ptr<node::For>>(node->stmt))
	{
		res = ConvertFor(std::get<std::unique_ptr<node::For>>(node->stmt), scope_id);
	}
	else if (std::holds_alternative<std::unique_ptr<node::Return>>(node->stmt))
	{
		res = ConvertReturn(std::get<std::unique_ptr<node::Return>>(node->stmt), scope_id);
	}
	else
	{
		return MakeError("unknown statement", *node);
	}

	if (is_new_scope)
		KillScope(scope_id);
	return res;
}

bool CodeGen::ConvertFuncDef(std::unique_ptr<node::FuncDef>& node, uint64_t scope_id)
{
	auto func_name = node->name;
	auto arg_num = node->args.size();
	Func func{ func_name, arg_num };

	if (func_info.find(func) != func_info.end())
	{
		return MakeError(("function " + func_name + "with " + std::to_string(arg_num) + "arguments is already defined")
			.c_str(), *node);
	}

	func_table.push_back(func);
	func_info[func] = FuncInfo{ func_cnt++, false, 0 };

	scope_id = GetNewScope();

	for (auto& arg : node->args)
	{
		auto var = GetVariableAddress(arg, scope_id, true);
		if (!std::get<0>(var))
			return false;
		auto var_address = std::get<1>(var);
		SetObj(var_address);
	}

	if (!ConvertStmt(node->stmt, scope_id))
		return false;

	KillScope(scope_id);

	return true;
}

bool CodeGen::ConvertFunc(std::unique_ptr<node::Func>& node, uint64_t scope_id)
{
	auto func_name = node->name;
	auto arg_num = node->args.size();

	if (func_info.find(Func{ func_name, arg_num }) == func_info.end())
	{
		return MakeError(("function " + func_name + "with " + std::to_string(arg_num) + "arguments is not defined")
			.c_str(), *node);
	}

	code.push_back(vm::Inst::ICR_FUNC_LEVEL);

	Push(0);
	auto& return_address = code[code.size() - 1];

	code.push_back(vm::Inst::PUSH_OBJ_IDX_OFFSET);
	code.push_back(vm::Inst::ADD_OBJ_IDX_OFFSET);
	code.push_back(var_cnt);
	code.push_back(vm::Inst::PUSH_START);

	for (uint64_t i = node->args.size() - 1; i >= 0; i--)
	{
		if (!ConvertExp(node->args[i], scope_id))
			return false;
	}

	Jump(0);

	return_address = code.size();

	auto func_id = func_info[Func{ func_name, arg_num }].func_id;
	unassigned_func_id[code.size() - 1] = func_id;

	return true;
}

bool CodeGen::ConvertBlock(std::unique_ptr<node::Block>& node, uint64_t scope_id)
{
	scope_id = GetNewScope();

	for (auto& stmt : node->stmts)
	{
		if (!ConvertStmt(stmt, scope_id))
			return false;
	}

	KillScope(scope_id);

	return true;
}

bool CodeGen::ConvertAssign(std::unique_ptr<node::Assign>& node, uint64_t scope_id)
{
	auto var = GetVariableAddress(node->var, scope_id, true);
	if (!std::get<0>(var))
		return false;
	auto var_address = std::get<1>(var);

	if (!ConvertExp(node->exp, scope_id))
		return false;

	SetObj(var_address);

	return true;
}

bool CodeGen::ConvertIf(std::unique_ptr<node::If>& node, uint64_t scope_id)
{
	auto condition_address = GetTempVariable();

	if (!ConvertExp(node->exp, scope_id))
		return false;

	code.push_back(vm::Inst::NOT);
	SetObj(condition_address);
	PushObj(condition_address);
	JumpIf(0);

	auto& else_start_address = code[code.size() - 1];

	if (!ConvertStmt(node->stmt, 0, true))
		return false;

	if (node->stmt_else != nullptr)
	{
		PushObj(condition_address);
		JumpIf(0);

		auto& if_end_address = code[code.size() - 1];

		else_start_address = code.size();

		if (!ConvertStmt(node->stmt_else, 0, true))
			return false;

		if_end_address = code.size();
	}
	else
	{
		else_start_address = code.size();
	}

	ReleaseTempVariable(condition_address);

	return true;
}

bool CodeGen::ConvertRepeat(std::unique_ptr<node::Repeat>& node, uint64_t scope_id)
{
	if (!ConvertExp(node->time, scope_id))
		return false;

	auto cnt_var = GetTempVariable();

	SetObj(cnt_var);

	auto return_address = code.size();

	Push0();
	PushObj(cnt_var);
	PushBOperator(vm::Inst::LESS_EQ);
	JumpIf(0);

	auto& break_address = code[code.size() - 1];

	if (!ConvertStmt(node->stmt, 0, true))
		return false;

	DecrementObj(cnt_var);
	Jump(return_address);

	break_address = code.size();

	return true;
}

bool CodeGen::ConvertFor(std::unique_ptr<node::For>& node, uint64_t scope_id)
{
	if (!ConvertExp(node->time, scope_id))
		return false;

	auto time_var = GetTempVariable();

	SetObj(time_var);

	auto cnt_var = GetVariableAddress(node->var, scope_id, true);
	if (!std::get<0>(cnt_var))
		return false;
	auto cnt_var_address = std::get<1>(cnt_var);

	SetObj0(cnt_var_address);

	auto return_address = code.size();

	PushObj(time_var);
	PushObj(cnt_var_address);
	PushBOperator(vm::Inst::GREATER_EQ);

	auto& break_address = code[code.size() - 1];

	if (!ConvertStmt(node->stmt, 0, true))
		return false;

	IncrementObj(cnt_var_address);
	Jump(return_address);

	break_address = code.size();

	ReleaseTempVariable(time_var);

	return true;
}

bool CodeGen::ConvertWhile(std::unique_ptr<node::While>& node, uint64_t scope_id)
{
	auto return_address = code.size();

	if (!ConvertExp(node->exp, scope_id))
		return false;

	code.push_back(vm::Inst::NOT);

	JumpIf(0);

	auto& while_end_address = code[code.size() - 1];

	if (!ConvertStmt(node->stmt, 0, true))
		return false;

	Jump(return_address);

	while_end_address = code.size();

	return true;
}

bool CodeGen::ConvertReturn(std::unique_ptr<node::Return>& node, uint64_t scope_id)
{
	if (!ConvertExp(node->exp, scope_id))
		return false;

	code.push_back(vm::Inst::PUT_RETURN_VALUE);
	code.push_back(vm::Inst::POP_TO_START);
	code.push_back(vm::Inst::SET_OBJ_IDX_OFFSET);
	code.push_back(vm::Inst::JUMP_TO_STACK_VALUE);
	code.push_back(vm::Inst::DCR_FUNC_LEVEL);
	code.push_back(vm::Inst::PUSH_RETURN_VALUE);

	return true;
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

	code.push_back(vm::Inst::BOPE);

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

bool CodeGen::ConvertVariable(std::unique_ptr<node::Variable>& node, uint64_t scope_id, bool declarable)
{
	auto var_address = GetVariableAddress(node, scope_id, declarable);
	if (!std::get<0>(var_address))
		return false;

	PushObj(std::get<1>(var_address));
	return true;
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

std::tuple<bool, uint64_t>
CodeGen::GetVariableAddress(const std::unique_ptr<node::Variable>& node, uint64_t scope_id, bool declarable)
{
	if (var_info.find(node->name) == var_info.end() || !is_scope_alive[var_info[node->name].scope_id])
	{
		if (!declarable)
			return std::make_tuple(MakeError(("variable " + node->name + " is not declared").c_str(), *node), 0);

		auto address = var_cnt++;
		var_info[node->name] = { address, scope_id };

		return std::make_tuple(true, address);
	}

	return std::make_tuple(true, var_info[node->name].address);
}

uint64_t CodeGen::GetTempVariable()
{
	return var_cnt++;
}

void CodeGen::ReleaseTempVariable(uint64_t address)
{
	if (var_cnt == address)
		var_cnt--;
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

void CodeGen::InitScope()
{
	scope_cnt = 1;
	is_scope_alive.clear();
	is_scope_alive[global_scope_id] = true;
}

uint64_t CodeGen::GetNewScope()
{
	is_scope_alive[scope_cnt] = true;
	return scope_cnt++;
}

void CodeGen::KillScope(uint64_t scope_id)
{
	is_scope_alive[scope_id] = false;
}

void CodeGen::InitFunc()
{
	func_cnt = 0;
	func_info.clear();
}

void CodeGen::InitVariable()
{
	var_cnt = 0;
	var_info.clear();
}

bool CodeGen::MakeError(const char* msg, const node::Node& node)
{
	printf("%s [%llu,%llu]\n", msg, node.pos.line, node.pos.col);

	return false;
}