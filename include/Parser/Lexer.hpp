//
// Created by Katsu Matsuda on 2020/04/17.
//

#ifndef _LEXER_HPP_
#define _LEXER_HPP_

#include <vector>
#include <string>

#include "AST/Token.hpp"

namespace xylo
{
	class Lexer
	{
	 private:
		struct SourceUnit
		{
			std::string source;

			SourcePos pos;
		};

		const std::string source;

		std::vector<SourceUnit> source_units;

		std::vector<Token> tokens;

		bool Divide();

		void PushSourceUnit(std::string& source, uint64_t line, uint64_t col);

		void PushToken(Token token);

	 public:
		struct Result
		{
			bool success;

			std::vector<Token> tokens;
		};

		Lexer(std::string source);

		Result Tokenize();
	};

}

#endif //_LEXER_HPP_
