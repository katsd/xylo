//
// Created by Katsu Matsuda on 2020/04/17.
//

#ifndef _LEXER_HPP_
#define _LEXER_HPP_

#include <vector>
#include <string>
#include <tuple>

#include "token/token.hpp"

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

		void PushSourceUnit(std::string& src, uint64_t line, uint64_t col);

		void PushToken(const Token& token);

		static std::tuple<bool, int64_t> Str2Int(const std::string& str);

		static std::tuple<bool, double> Str2Float(const std::string& str);

	 public:
		struct Result
		{
			bool success;

			std::vector<Token> tokens;
		};

		explicit Lexer(std::string source);

		Result Tokenize();
	};

}

#endif //_LEXER_HPP_
