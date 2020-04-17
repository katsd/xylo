//
// Created by Katsu Matsuda on 2020/04/17.
//

#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

#include <string>

#include "AST/TokenType.hpp"
#include "AST/TokenValue.hpp"
#include "AST/SourcePos.hpp"

namespace xylo
{
	struct Token
	{
	 private:
		TokenValue value;

		std::string source;

	 public:
		TokenType type;

		SourcePos pos;

		uint64_t GetInt();

		double GetFloat();

		std::string GetString();

		Symbol GetSymbol();

		Reserved GetReserved();

		std::string GetName();

	};
}

#endif //_TOKEN_HPP_
