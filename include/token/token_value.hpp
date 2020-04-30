//
// Created by Katsu Matsuda on 2020/04/17.
//

#ifndef _TOKEN_VALUE_HPP_
#define _TOKEN_VALUE_HPP_

#include <cstdint>

#include "attribute.hpp"
#include "reserved.hpp"
#include "symbol.hpp"

namespace xylo
{
union TokenValue
{
	int64_t ival;

	double dval;

	Attribute attribute;

	Reserved reserved;

	Symbol symbol;

	TokenValue()
	{
	}

	TokenValue(int64_t ival)
	{
		this->ival = ival;
	}

	TokenValue(double dval)
	{
		this->dval = dval;
	}

	TokenValue(Attribute attribute)
	{
		this->attribute = attribute;
	}

	TokenValue(Reserved reserved)
	{
		this->reserved = reserved;
	}

	TokenValue(Symbol symbol)
	{
		this->symbol = symbol;
	}
};
}

#endif //_TOKEN_VALUE_HPP_
