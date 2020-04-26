//
// Created by Katsu Matsuda on 2020/04/20.
//

#ifndef _CODEGEN_HPP_
#define _CODEGEN_HPP_

#include <cstdint>
#include <map>
#include <memory>
#include <vector>
#include <unordered_map>

#include "AST/AST.hpp"
#include "VM/VM.hpp"

namespace xylo
{
class CodeGen
{
 private:
	struct VariableInfo
	{
		uint64_t address;

		uint64_t scope_id;
	};

	struct Func
	{
		std::string name;

		uint64_t arg_num;

		bool operator<(const Func& func) const
		{
			return std::tie(name, arg_num) < std::tie(func.name, func.arg_num);
		}
	};

	struct FuncInfo
	{
		uint64_t func_id;

		uint64_t start_address;

		bool is_native;

		uint64_t native_func_id;
	};

	const std::unique_ptr<ast::Root>& ast;

	std::vector<uint64_t> code;

	// #########################################
	// #########################################

	// const

	std::vector<vm::Obj> const_table;

	std::map<vm::Obj, uint64_t> const_address;

	// #########################################
	// #########################################

	// scope

	static constexpr uint64_t global_scope_id = 0;

	uint64_t scope_cnt;

	std::map<uint64_t, bool> is_scope_alive;

	// #########################################
	// #########################################

	// variable

	uint64_t global_var_cnt;

	uint64_t var_cnt;

	std::map<std::string, VariableInfo> var_info;

	// #########################################
	// #########################################

	// func
	uint64_t func_cnt;

	std::vector<Func> func_table;

	std::map<Func, FuncInfo> func_info;

	// code address, func id
	std::map<uint64_t, uint64_t> unassigned_func_id;

	// #########################################
	// #########################################

	bool ConvertRoot(const std::unique_ptr<ast::Root>& node, uint64_t scope_id);

	bool ConvertStmt(const std::unique_ptr<ast::Stmt>& node, uint64_t scope_id, bool is_new_scope = false);

	bool ConvertFuncDef(const std::unique_ptr<ast::FuncDef>& node, uint64_t scope_id);

	bool ConvertFunc(const std::unique_ptr<ast::Func>& node, uint64_t scope_id);

	bool ConvertBlock(const std::unique_ptr<ast::Block>& node, uint64_t scope_id);

	bool ConvertAssign(const std::unique_ptr<ast::Assign>& node, uint64_t scope_id);

	bool ConvertIf(const std::unique_ptr<ast::If>& node, uint64_t scope_id);

	bool ConvertRepeat(const std::unique_ptr<ast::Repeat>& node, uint64_t scope_id);

	bool ConvertFor(const std::unique_ptr<ast::For>& node, uint64_t scope_id);

	bool ConvertWhile(const std::unique_ptr<ast::While>& node, uint64_t scope_id);

	bool ConvertReturn(const std::unique_ptr<ast::Return>& node, uint64_t scope_id);

	bool ConvertExp(const std::unique_ptr<ast::Exp>& node, uint64_t scope_id);

	bool ConvertBOperator(const std::unique_ptr<ast::BOperator>& node, uint64_t scope_id);

	bool ConvertUOperator(const std::unique_ptr<ast::UOperator>& node, uint64_t scope_id);

	bool ConvertValue(const std::unique_ptr<ast::Value>& node, uint64_t scope_id);

	bool ConvertVariable(const std::unique_ptr<ast::Variable>& node, uint64_t scope_id, bool declarable = false);

	bool ConvertInt(const std::unique_ptr<ast::Int>& node);

	bool ConvertFloat(const std::unique_ptr<ast::Float>& node);

	bool ConvertString(const std::unique_ptr<ast::String>& node);

	bool DefineFunc(const std::unique_ptr<ast::FuncDef>& node);

	std::tuple<bool, uint64_t>
	GetVariableAddress(const std::unique_ptr<ast::Variable>& node, uint64_t scope_id, bool declarable);

	uint64_t GetTempVariable();

	void ReleaseTempVariable(uint64_t address);

	inline void Push(uint64_t value)
	{
		code.push_back(vm::Inst::PUSH);
		code.push_back(value);
	}

	inline void PushObj(uint64_t address)
	{
		code.push_back(vm::Inst::PUSH_OBJ);
		code.push_back(address);
	}

	inline void PushGlobalObj(uint64_t address)
	{
		code.push_back(vm::Inst::PUSH_GLOBAL_OBJ);
		code.push_back(address);
	}

	inline void PushConst(vm::Obj obj)
	{
		code.push_back(vm::Inst::PUSH_CONST);
		code.push_back(AddConst(obj));
	}

	inline void Push0()
	{
		code.push_back(vm::Inst::PUSH_ZERO);
	}

	inline void Push1()
	{
		code.push_back(vm::Inst::PUSH_ONE);
	}

	inline void SetObj(uint64_t address)
	{
		code.push_back(vm::Inst::SET_OBJ);
		code.push_back(address);
	}

	inline void SetGlobalObj(uint64_t address)
	{
		code.push_back(vm::Inst::SET_GLOBAL_OBJ);
		code.push_back(address);
	}

	inline void SetObj0(uint64_t address)
	{
		code.push_back(vm::Inst::SET_OBJ_ZERO);
		code.push_back(address);
	}

	inline void SetObj1(uint64_t address)
	{
		code.push_back(vm::Inst::SET_OBJ_ONE);
		code.push_back(address);
	}

	inline void IncrementObj(uint64_t address)
	{
		code.push_back(vm::Inst::ICR_OBJ);
		code.push_back(address);
	}

	inline void DecrementObj(uint64_t address)
	{
		code.push_back(vm::Inst::DCR_OBJ);
		code.push_back(address);
	}

	inline void Jump(uint64_t address)
	{
		code.push_back(vm::Inst::JUMP);
		code.push_back(address);
	}

	inline void JumpIf(uint64_t address)
	{
		code.push_back(vm::Inst::JUMP_IF);
		code.push_back(address);
	}

	inline void PushBOperator(vm::Inst ope)
	{
		code.push_back(vm::Inst::BOPE);
		code.push_back(ope);
	}

	void InitConstTable();

	// Return const address
	uint64_t AddConst(vm::Obj& obj);

	void InitScope();

	uint64_t GetNewScope();

	void KillScope(uint64_t scope_id);

	void InitFunc();

	void InitGlobalVariable();

	void InitVariable();

	static bool MakeError(const char* msg, const ast::Node& node);

 public:
	struct Result
	{
		bool success;

		std::vector<uint64_t> code;

		std::vector<vm::Obj> const_table;
	};

	Result GenerateCode();

	CodeGen(const std::unique_ptr<ast::Root>& ast)
		: ast(ast)
	{
	}
};
}

#endif //_CODEGEN_HPP_
