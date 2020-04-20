//
// Created by Katsu Matsuda on 2020/04/20.
//

#ifndef _CODEGEN_HPP_
#define _CODEGEN_HPP_

#include <cstdint>
#include <memory>
#include <vector>

#include "AST/AST.hpp"

namespace xylo
{
class CodeGen
{
 private:
	const std::unique_ptr<node::Root>& ast;

	std::vector<uint64_t> code;

 public:
	std::vector<uint64_t> GenerateCode();

	CodeGen(const std::unique_ptr<node::Root>& ast)
		: ast(ast)
	{
	}
};
}

#endif //_CODEGEN_HPP_
