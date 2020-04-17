//
// Created by Katsu Matsuda on 2020/04/17.
//

#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

#include <string>

#include "TokenType.hpp"
#include "TokenValue.hpp"
#include "SourcePos.hpp"

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

		inline void Set(int64_t ival, const std::string& src)
		{
			type = TokenType::INT;
			value = ival;
		}

		inline void Set(double dval, const std::string& src)
		{
			type = TokenType::FLOAT;
			value = dval;
		}

		// "**" -> STRING
		// **** -> NAME
		inline void Set(const std::string& src)
		{
			if (!src.empty() && src[0] == '"' && src[src.size() - 1] == '"')
			{
				type = TokenType::STRING;
				source = src.substr(1, src.size() - 2);
			}
			else
			{
				type = TokenType::NAME;
				source = src;
			}
		}

		inline void Set(Symbol symbol, const std::string& src)
		{
			type = TokenType::SYMBOL;
			value = symbol;
			source = src;
		}

		inline void Set(Reserved reserved, const std::string& src)
		{
			type = TokenType::RESERVED;
			value = reserved;
			source = src;
		}

	};
}

#endif //_TOKEN_HPP_
