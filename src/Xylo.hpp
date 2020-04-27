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

using namespace xylo;

class Xylo
{
 private:
	std::unique_ptr<ast::Root> ast;

	std::unique_ptr<vm::VM> eval;

 public:
	explicit Xylo(const std::string& source)
	{
		ast = Parser(source).Parse();

		if (ast == nullptr)
			return;

		auto codegen_result = CodeGen(ast).GenerateCode();

		if (!codegen_result.success)
			return;

		eval = std::make_unique<vm::VM>(codegen_result.code, codegen_result.const_table);
	}

	void Run()
	{
		eval->Run();
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
