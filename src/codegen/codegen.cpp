//
// Created by Katsu Matsuda on 2020/04/20.
//

#include "codegen/codegen.hpp"

using namespace xylo;

CodeGen::Result CodeGen::GenerateCode()
{
	code.clear();

	InitGlobalVariable();
	InitVariable();
	InitConstTable();
	InitScope();
	InitFunc();

	if (!ConvertRoot(ast, global_scope_id))
	{
		printf("failed to generate code\n");
		return Result{ false };
	}

	std::map<std::string, uint64_t> func_start_idx;
	for (const auto& info : func_info)
	{
		if (!info.second.is_native)
			func_start_idx[info.first.name + "_" + std::to_string(info.first.arg_num)] = info.second.start_address;
	}

	return Result{ true, code, const_table, func_start_idx };
}

bool CodeGen::ConvertRoot(const std::unique_ptr<ast::Root>& node, uint64_t scope_id)
{
	for (auto& stmt : node->stmts)
	{
		if (!std::holds_alternative<std::unique_ptr<ast::FuncDef>>(stmt))
			continue;

		if (!DefineFunc(std::get<std::unique_ptr<ast::FuncDef>>(stmt)))
			return false;
	}

	code.push_back(vm::Inst::START);

	for (auto& stmt : node->stmts)
	{
		if (!std::holds_alternative<std::unique_ptr<ast::Stmt>>(stmt))
			continue;

		if (!ConvertStmt(std::get<std::unique_ptr<ast::Stmt>>(stmt), scope_id))
			return false;
	}

	code.push_back(vm::Inst::END);

	for (auto& stmt : node->stmts)
	{
		if (!std::holds_alternative<std::unique_ptr<ast::FuncDef>>(stmt))
			continue;

		if (!ConvertFuncDef(std::get<std::unique_ptr<ast::FuncDef>>(stmt), scope_id))
			return false;
	}

	for (auto i : unassigned_func_id)
	{
		code[i.first] = func_info[func_table[i.second]].start_address;
	}

	return true;
}

bool CodeGen::ConvertStmt(const std::unique_ptr<ast::Stmt>& node, uint64_t scope_id, bool is_new_scope)
{
	auto cur_var_cnt = var_cnt;

	if (is_new_scope)
		scope_id = GetNewScope();

	bool res;

	if (std::holds_alternative<std::unique_ptr<ast::Block>>(node->stmt))
	{
		res = ConvertBlock(std::get<std::unique_ptr<ast::Block>>(node->stmt), scope_id);
	}
	else if (std::holds_alternative<std::unique_ptr<ast::Assign>>(node->stmt))
	{
		res = ConvertAssign(std::get<std::unique_ptr<ast::Assign>>(node->stmt), scope_id);
	}
	else if (std::holds_alternative<std::unique_ptr<ast::Func>>(node->stmt))
	{
		res = ConvertFunc(std::get<std::unique_ptr<ast::Func>>(node->stmt), scope_id);
	}
	else if (std::holds_alternative<std::unique_ptr<ast::If>>(node->stmt))
	{
		res = ConvertIf(std::get<std::unique_ptr<ast::If>>(node->stmt), scope_id);
	}
	else if (std::holds_alternative<std::unique_ptr<ast::While>>(node->stmt))
	{
		res = ConvertWhile(std::get<std::unique_ptr<ast::While>>(node->stmt), scope_id);
	}
	else if (std::holds_alternative<std::unique_ptr<ast::Repeat>>(node->stmt))
	{
		res = ConvertRepeat(std::get<std::unique_ptr<ast::Repeat>>(node->stmt), scope_id);
	}
	else if (std::holds_alternative<std::unique_ptr<ast::For>>(node->stmt))
	{
		res = ConvertFor(std::get<std::unique_ptr<ast::For>>(node->stmt), scope_id);
	}
	else if (std::holds_alternative<std::unique_ptr<ast::Return>>(node->stmt))
	{
		res = ConvertReturn(std::get<std::unique_ptr<ast::Return>>(node->stmt), scope_id);
	}
	else
	{
		return MakeError("unknown statement", *node);
	}

	if (is_new_scope)
	{
		var_cnt = cur_var_cnt;
		KillScope(scope_id);
	}

	return res;
}

bool CodeGen::DefineFunc(const std::unique_ptr<ast::FuncDef>& node)
{
	std::string func_name = node->name;
	auto arg_num = node->args.size();
	Func func{ func_name, arg_num };

	if (func_info.find(func) != func_info.end())
	{
		return MakeError(("function " + func_name + " with " + std::to_string(arg_num)
						  + " arguments is already defined")
			.c_str(), *node);
	}

	func_table.push_back(func);

	uint64_t func_id;
	if (node->is_native_func)
		func_id = node->native_func_id;
	else
		func_id = func_cnt++;

	func_info[func] = FuncInfo{ func_id, 0, node->is_native_func, node->native_func_id };

	return true;
}

bool CodeGen::ConvertFuncDef(const std::unique_ptr<ast::FuncDef>& node, uint64_t scope_id)
{
	Func func{ node->name, node->args.size() };
	func_info[func].start_address = code.size();

	scope_id = GetNewScope();

	code.push_back(vm::Inst::START);

	InitVariable();

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

	code.push_back(vm::Inst::END);

	KillScope(scope_id);

	return true;
}

bool CodeGen::ConvertFunc(const std::unique_ptr<ast::Func>& node, uint64_t scope_id)
{
	auto func_name = node->name;
	auto arg_num = node->args.size();

	if (func_name == "print" && arg_num == 1)
	{
		if (!ConvertExp(node->args[0], scope_id))
			return false;

		code.push_back(vm::Inst::OUT);
		return true;
	}

	if (func_name == "input" && arg_num == 0)
	{
		code.push_back(vm::Inst::INPUT);
		return true;
	}

	if (func_info.find(Func{ func_name, arg_num }) == func_info.end())
	{
		return MakeError(("function " + func_name + " with " + std::to_string(arg_num) + " arguments is not defined")
			.c_str(), *node);
	}

	auto info = func_info[Func{ func_name, arg_num }];

	if (info.is_native)
	{
		for (int64_t i = node->args.size() - 1; i >= 0; i--)
		{
			if (!ConvertExp(node->args[i], scope_id))
				return false;
		}

		CallNative(info.func_id, node->args.size());

		return true;
	}

	code.push_back(vm::Inst::ICR_FUNC_LEVEL);

	Push(0);
	auto return_address_idx = code.size() - 1;

	code.push_back(vm::Inst::PUSH_OBJ_IDX_OFFSET);
	code.push_back(vm::Inst::PUSH_START);

	for (int64_t i = node->args.size() - 1; i >= 0; i--)
	{
		if (!ConvertExp(node->args[i], scope_id))
			return false;
	}

	AddObjIdxOffset(var_cnt);

	Jump(0);

	code[return_address_idx] = code.size();

	unassigned_func_id[code.size() - 1] = info.func_id;

	code.push_back(vm::Inst::PUSH_RETURN_VALUE);

	return true;
}

bool CodeGen::ConvertBlock(const std::unique_ptr<ast::Block>& node, uint64_t scope_id)
{
	auto cur_var_cnt = var_cnt;
	scope_id = GetNewScope();

	for (auto& stmt : node->stmts)
	{
		if (!ConvertStmt(stmt, scope_id))
			return false;
	}

	var_cnt = cur_var_cnt;
	KillScope(scope_id);

	return true;
}

bool CodeGen::ConvertAssign(const std::unique_ptr<ast::Assign>& node, uint64_t scope_id)
{
	auto var = GetVariableAddress(node->var, scope_id, true);
	if (!std::get<0>(var))
		return false;
	auto var_address = std::get<1>(var);
	auto is_global_variable = std::get<2>(var);

	if (!ConvertExp(node->exp, scope_id))
		return false;

	if (is_global_variable)
		SetGlobalObj(var_address);
	else
		SetObj(var_address);

	return true;
}

bool CodeGen::ConvertIf(const std::unique_ptr<ast::If>& node, uint64_t scope_id)
{
	auto condition_address = GetTempVariable();

	if (!ConvertExp(node->exp, scope_id))
		return false;

	SetObj(condition_address);
	PushObj(condition_address);
	code.push_back(vm::Inst::NOT);
	JumpIf(0);

	auto else_start_address_idx = code.size() - 1;

	if (!ConvertStmt(node->stmt, 0, true))
		return false;

	if (node->stmt_else != nullptr)
	{
		PushObj(condition_address);
		JumpIf(0);

		auto if_end_address_idx = code.size() - 1;

		code[else_start_address_idx] = code.size();

		if (!ConvertStmt(node->stmt_else, 0, true))
			return false;

		code[if_end_address_idx] = code.size();
	}
	else
	{
		code[else_start_address_idx] = code.size();
	}

	ReleaseTempVariable(condition_address);

	return true;
}

bool CodeGen::ConvertRepeat(const std::unique_ptr<ast::Repeat>& node, uint64_t scope_id)
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

	auto break_address_idx = code.size() - 1;

	if (!ConvertStmt(node->stmt, 0, true))
		return false;

	DecrementObj(cnt_var);
	Jump(return_address);

	code[break_address_idx] = code.size();

	return true;
}

bool CodeGen::ConvertFor(const std::unique_ptr<ast::For>& node, uint64_t scope_id)
{
	if (!ConvertExp(node->time, scope_id))
		return false;

	scope_id = GetNewScope();

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
	JumpIf(0);
	auto break_address_idx = code.size() - 1;

	if (!ConvertStmt(node->stmt, scope_id, true))
		return false;

	IncrementObj(cnt_var_address);
	Jump(return_address);

	code[break_address_idx] = code.size();

	ReleaseTempVariable(time_var);

	KillScope(scope_id);

	return true;
}

bool CodeGen::ConvertWhile(const std::unique_ptr<ast::While>& node, uint64_t scope_id)
{
	auto return_address = code.size();

	if (!ConvertExp(node->exp, scope_id))
		return false;

	code.push_back(vm::Inst::NOT);

	JumpIf(0);

	auto while_end_address_idx = code.size() - 1;

	if (!ConvertStmt(node->stmt, 0, true))
		return false;

	Jump(return_address);

	code[while_end_address_idx] = code.size();

	return true;
}

bool CodeGen::ConvertReturn(const std::unique_ptr<ast::Return>& node, uint64_t scope_id)
{
	if (!ConvertExp(node->exp, scope_id))
		return false;

	code.push_back(vm::Inst::PUT_RETURN_VALUE);
	code.push_back(vm::Inst::POP_TO_START);
	code.push_back(vm::Inst::SET_OBJ_IDX_OFFSET);
	code.push_back(vm::Inst::DCR_FUNC_LEVEL);
	code.push_back(vm::Inst::JUMP_TO_STACK_VALUE);

	return true;
}

bool CodeGen::ConvertExp(const std::unique_ptr<ast::Exp>& node, uint64_t scope_id)
{
	if (std::holds_alternative<std::unique_ptr<ast::Value>>(node->value))
		return ConvertValue(std::get<std::unique_ptr<ast::Value>>(node->value), scope_id);

	if (std::holds_alternative<std::unique_ptr<ast::BOperator>>(node->value))
		return ConvertBOperator(std::get<std::unique_ptr<ast::BOperator>>(node->value), scope_id);

	if (std::holds_alternative<std::unique_ptr<ast::UOperator>>(node->value))
		return ConvertUOperator(std::get<std::unique_ptr<ast::UOperator>>(node->value), scope_id);

	if (std::holds_alternative<std::unique_ptr<ast::Func>>(node->value))
		return ConvertFunc(std::get<std::unique_ptr<ast::Func>>(node->value), scope_id);

	return MakeError("unknown expression", *node);
}

bool CodeGen::ConvertBOperator(const std::unique_ptr<ast::BOperator>& node, uint64_t scope_id)
{
	if (!ConvertExp(node->right, scope_id))
		return false;

	if (!ConvertExp(node->left, scope_id))
		return false;

	code.push_back(vm::Inst::BOPE);

	switch (node->type)
	{
	case ast::BOperatorType::OR:
		code.push_back(vm::Inst::OR);
		break;
	case ast::BOperatorType::AND:
		code.push_back(vm::Inst::AND);
		break;
	case ast::BOperatorType::BIN_OR:
		code.push_back(vm::Inst::BIN_OR);
		break;
	case ast::BOperatorType::BIN_XOR:
		code.push_back(vm::Inst::BIN_XOR);
		break;
	case ast::BOperatorType::BIN_AND:
		code.push_back(vm::Inst::BIN_AND);
		break;
	case ast::BOperatorType::EQUAL:
		code.push_back(vm::Inst::EQUAL);
		break;
	case ast::BOperatorType::NOT_EQUAL:
		code.push_back(vm::Inst::NOT_EQUAL);
		break;
	case ast::BOperatorType::LESS:
		code.push_back(vm::Inst::LESS);
		break;
	case ast::BOperatorType::LESS_EQ:
		code.push_back(vm::Inst::LESS_EQ);
		break;
	case ast::BOperatorType::GREATER:
		code.push_back(vm::Inst::GREATER);
		break;
	case ast::BOperatorType::GREATER_EQ:
		code.push_back(vm::Inst::GREATER_EQ);
		break;
	case ast::BOperatorType::ADD:
		code.push_back(vm::Inst::ADD);
		break;
	case ast::BOperatorType::SUB:
		code.push_back(vm::Inst::SUB);
		break;
	case ast::BOperatorType::MUL:
		code.push_back(vm::Inst::MUL);
		break;
	case ast::BOperatorType::DIV:
		code.push_back(vm::Inst::DIV);
		break;
	case ast::BOperatorType::MOD:
		code.push_back(vm::Inst::MOD);
		break;
	default:
		return MakeError("unknown operator", *node);
	}

	return true;
}

bool CodeGen::ConvertUOperator(const std::unique_ptr<ast::UOperator>& node, uint64_t scope_id)
{
	if (!ConvertExp(node->exp, scope_id))
		return false;

	switch (node->type)
	{
	case ast::UOperatorType::MINUS:
		code.push_back(vm::Inst::MINUS);
		break;
	case ast::UOperatorType::NOT:
		code.push_back(vm::Inst::NOT);
		break;
	case ast::UOperatorType::BIN_NOT:
		code.push_back(vm::Inst::BIN_NOT);
		break;
	}

	return true;
}

bool CodeGen::ConvertValue(const std::unique_ptr<ast::Value>& node, uint64_t scope_id)
{
	if (std::holds_alternative<std::unique_ptr<ast::Int>>(node->value))
		return ConvertInt(std::get<std::unique_ptr<ast::Int>>(node->value));

	if (std::holds_alternative<std::unique_ptr<ast::Float>>(node->value))
		return ConvertFloat(std::get<std::unique_ptr<ast::Float>>(node->value));

	if (std::holds_alternative<std::unique_ptr<ast::String>>(node->value))
		return ConvertString(std::get<std::unique_ptr<ast::String>>(node->value));

	if (std::holds_alternative<std::unique_ptr<ast::Variable>>(node->value))
		return ConvertVariable(std::get<std::unique_ptr<ast::Variable>>(node->value), scope_id);

	return MakeError("unknown value", *node);
}

bool CodeGen::ConvertVariable(const std::unique_ptr<ast::Variable>& node, uint64_t scope_id, bool declarable)
{
	auto var_address = GetVariableAddress(node, scope_id, declarable);
	if (!std::get<0>(var_address))
		return false;

	auto address = std::get<1>(var_address);

	if (std::get<2>(var_address))
		PushGlobalObj(address);
	else
		PushObj(address);

	return true;
}

bool CodeGen::ConvertInt(const std::unique_ptr<ast::Int>& node)
{
	PushConst(vm::Obj{ node->value });
	return true;
}

bool CodeGen::ConvertFloat(const std::unique_ptr<ast::Float>& node)
{
	PushConst(vm::Obj{ node->value });
	return true;
}

bool CodeGen::ConvertString(const std::unique_ptr<ast::String>& node)
{
	PushConst(vm::Obj{ node->value });
	return true;
}

void CodeGen::InitConstTable()
{
	const_table.clear();
	const_address.clear();
}

std::tuple<bool, uint64_t, bool>
CodeGen::GetVariableAddress(const std::unique_ptr<ast::Variable>& node, uint64_t scope_id, bool declarable)
{
	if (var_info.find(node->name) == var_info.end() || !is_scope_alive[var_info[node->name].scope_id])
	{
		if (!declarable)
			return std::make_tuple(MakeError(("variable " + node->name + " is not declared").c_str(), *node), 0, false);

		uint64_t address;
		if (scope_id == global_scope_id)
			address = global_var_cnt++;
		else
			address = var_cnt++;
		var_info[node->name] = { address, scope_id };

		return std::make_tuple(true, address, scope_id == global_scope_id);
	}

	auto info = var_info[node->name];

	return std::make_tuple(true, info.address, info.scope_id == global_scope_id);
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

void CodeGen::InitGlobalVariable()
{
	global_var_cnt = 0;
}

void CodeGen::InitVariable()
{
	var_cnt = 0;
	var_info.clear();
}

bool CodeGen::MakeError(const char* msg, const ast::Node& node)
{
	printf("%s [%llu,%llu]\n", msg, node.pos.line + 1, node.pos.col + 1);

	return false;
}