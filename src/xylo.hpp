//
// xylo.hpp
// xylo
//
// Copyright (c) 2019 Katsu Matsuda. All rights reserved.
//

#ifndef XYLO_HPP_
#define XYLO_HPP_

#include <cstdint>
#include <string>

#include "Parser/Parser.hpp"
#include "CodeGen/CodeGen.hpp"
#include "VM/VM.hpp"

using namespace xylo;

class Xylo
{
 private:

 public:
	explicit Xylo(const std::string& source)
	{
		auto ast = Parser(source).Parse();

		if (ast == nullptr)
			return;

		puts(ast->Node2Str(0).c_str());

		const auto code = CodeGen(ast).GenerateCode();

		vm::VM eval{ code };
		eval.OutCode();
	}
};

#endif /* XYLO_HPP_ */
