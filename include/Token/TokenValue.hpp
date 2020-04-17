//
// Created by Katsu Matsuda on 2020/04/17.
//

#ifndef _TOKENVALUE_HPP_
#define _TOKENVALUE_HPP_

#include <cstdint>

#include "Reserved.hpp"
#include "Symbol.hpp"

namespace xylo
{
	union TokenValue
	{
		int64_t ival;

		double dval;

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

#endif //_TOKENVALUE_HPP_
