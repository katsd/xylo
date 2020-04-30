//
// Created by Katsu Matsuda on 2020/04/17.
//

#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

#include <string>

#include "token_type.hpp"
#include "token_value.hpp"
#include "source_pos.hpp"

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

	[[nodiscard]]
	inline int64_t GetInt() const
	{
		return value.ival;
	}

	[[nodiscard]]
	inline double GetFloat() const
	{
		return value.dval;
	}

	[[nodiscard]]
	inline std::string GetString() const
	{
		return source;
	}

	[[nodiscard]]
	inline Symbol GetSymbol() const
	{
		return value.symbol;
	}

	[[nodiscard]]
	inline Attribute GetAttribute() const
	{
		return value.attribute;
	}

	[[nodiscard]]
	inline Reserved GetReserved() const
	{
		return value.reserved;
	}

	[[nodiscard]]
	inline std::string GetIdentifier() const
	{
		return source;
	}

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
	// **** -> Identifier
	inline void Set(const std::string& src)
	{
		if (!src.empty() && src[0] == '"' && src[src.size() - 1] == '"')
		{
			type = TokenType::STRING;
			source = src.substr(1, src.size() - 2);
		}
		else
		{
			type = TokenType::IDENTIFIER;
			source = src;
		}
	}

	inline void Set(Symbol symbol, const std::string& src)
	{
		type = TokenType::SYMBOL;
		value = symbol;
		source = src;
	}

	inline void Set(Attribute attribute, const std::string src)
	{
		type = TokenType::ATTRIBUTE;
		value = attribute;
		source = src;
	}

	inline void Set(Reserved reserved, const std::string& src)
	{
		type = TokenType::RESERVED;
		value = reserved;
		source = src;
	}

	static std::string Symbol2Str(Symbol symbol);

	static std::string Reserved2Str(Reserved reserved);

};
}

#endif //_TOKEN_HPP_
