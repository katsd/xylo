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

bool CodeGen::ConvertRoot(std::unique_ptr<node::Root> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertStmt(std::unique_ptr<node::Stmt> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertFuncDef(std::unique_ptr<node::FuncDef> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertFunc(std::unique_ptr<node::Func> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertBlock(std::unique_ptr<node::Block> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertAssign(std::unique_ptr<node::Assign> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertIf(std::unique_ptr<node::If> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertRepeat(std::unique_ptr<node::Repeat> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertFor(std::unique_ptr<node::For> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertWhile(std::unique_ptr<node::While> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertReturn(std::unique_ptr<node::Return> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertExp(std::unique_ptr<node::Exp> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertBOperator(std::unique_ptr<node::BOperator> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertUOperator(std::unique_ptr<node::UOperator> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertValue(std::unique_ptr<node::Value> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertVariable(std::unique_ptr<node::Variable> node, uint64_t scope_id)
{

}

bool CodeGen::ConvertInt(std::unique_ptr<node::Int> node)
{
	PushConst(vm::Obj{ node->value });
}

bool CodeGen::ConvertFloat(std::unique_ptr<node::Float> node)
{
	PushConst(vm::Obj{ node->value });
}

bool CodeGen::ConvertString(std::unique_ptr<node::String> node)
{
	PushConst(vm::Obj{ node->value });
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
