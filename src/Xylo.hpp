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

#include "Parser/Parser.hpp"
#include "CodeGen/CodeGen.hpp"
#include "VM/VM.hpp"
#include "Native/Native.hpp"

using namespace xylo;

class Xylo
{
 private:
	std::unique_ptr<ast::Root> ast;

	std::unique_ptr<vm::VM> eval;

 public:
	explicit Xylo(const std::string& source, const std::vector<native::Native::Func>& ext_ilb = {})
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

		eval = std::make_unique<vm::VM>(codegen_result.code, codegen_result.const_table, codegen_result.func_start_idx);
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
