//
// Created by Katsu Matsuda on 2020/04/20.
//

#ifndef _CODEGEN_HPP_
#define _CODEGEN_HPP_

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "AST/AST.hpp"
#include "VM/VM.hpp"

namespace xylo
{
class CodeGen
{
 private:
	static constexpr uint64_t global_scope_id = 0;

	const std::unique_ptr<node::Root>& ast;

	std::vector<uint64_t> code;

	std::vector<vm::Obj> const_table;

	std::map<vm::Obj, uint64_t> const_address;

	bool ConvertRoot(std::unique_ptr<node::Root>& node, uint64_t scope_id);

	bool ConvertStmt(std::unique_ptr<node::Stmt>& node, uint64_t scope_id);

	bool ConvertFuncDef(std::unique_ptr<node::FuncDef>& node, uint64_t scope_id);

	bool ConvertFunc(std::unique_ptr<node::Func>& node, uint64_t scope_id);

	bool ConvertBlock(std::unique_ptr<node::Block>& node, uint64_t scope_id);

	bool ConvertAssign(std::unique_ptr<node::Assign>& node, uint64_t scope_id);

	bool ConvertIf(std::unique_ptr<node::If>& node, uint64_t scope_id);

	bool ConvertRepeat(std::unique_ptr<node::Repeat>& node, uint64_t scope_id);

	bool ConvertFor(std::unique_ptr<node::For>& node, uint64_t scope_id);

	bool ConvertWhile(std::unique_ptr<node::While>& node, uint64_t scope_id);

	bool ConvertReturn(std::unique_ptr<node::Return>& node, uint64_t scope_id);

	bool ConvertExp(std::unique_ptr<node::Exp>& node, uint64_t scope_id);

	bool ConvertBOperator(std::unique_ptr<node::BOperator>& node, uint64_t scope_id);

	bool ConvertUOperator(std::unique_ptr<node::UOperator>& node, uint64_t scope_id);

	bool ConvertValue(std::unique_ptr<node::Value>& node, uint64_t scope_id);

	bool ConvertVariable(std::unique_ptr<node::Variable>& node, uint64_t scope_id);

	bool ConvertInt(std::unique_ptr<node::Int>& node);

	bool ConvertFloat(std::unique_ptr<node::Float>& node);

	bool ConvertString(std::unique_ptr<node::String>& node);

	inline void PushConst(vm::Obj obj)
	{
		code.push_back(vm::Inst::PUSH_CONST);
		code.push_back(AddConst(obj));
	}

	void InitConstTable();

	// Return const address
	uint64_t AddConst(vm::Obj& obj);

	bool MakeError(const char* msg, const node::Node& node);

 public:
	std::vector<uint64_t> GenerateCode();

	CodeGen(const std::unique_ptr<node::Root>& ast)
		: ast(ast)
	{
	}
};
}

#endif //_CODEGEN_HPP_
