//
// xylo.hpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#ifndef XYLO_HPP_
#define XYLO_HPP_

#include <cstdint>
#include <memory>
#include <string>

#include "parser/parser.hpp"
#include "codegen/codegen.hpp"
#include "vm/vm.hpp"
#include "native/native.hpp"

using namespace xylo;

class Xylo
{
 private:
	const void* ext_xylo_instance;

	std::unique_ptr<ast::Root> ast;

	std::unique_ptr<vm::VM> eval;

 public:
	explicit Xylo(const std::string& source, const void* ext_xylo_instance = nullptr, const std::vector<native::Native::Func>& ext_ilb = {})
		: ext_xylo_instance(ext_xylo_instance)
	{
		for (const auto& func : ext_ilb)
			native::Native::AddFunc(func);

		native::Native::Init();

		auto std_lib = native::Native::StandardLibraryCode();

		ast = Parser(source + std_lib).Parse();

		if (ast == nullptr)
			return;

		auto codegen_result = CodeGen(ast).GenerateCode();

		if (!codegen_result.success)
			return;

		eval = std::make_unique<vm::VM>(
			codegen_result.code, codegen_result.const_table, codegen_result.func_start_idx, ext_xylo_instance);
	}

	void Run()
	{
		eval->Run();
	}

	void Run(const std::string& func_name, const std::vector<vm::Obj>& args)
	{
		eval->Run(func_name, args);
	}

	void OutAST()
	{
		puts(ast->Node2Str(0).c_str());
	}

	void OutCode()
	{
		eval->OutCode();
	}
};

#endif /* XYLO_HPP_ */
