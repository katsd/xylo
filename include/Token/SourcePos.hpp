//
// Created by Katsu Matsuda on 2020/04/17.
//

#ifndef _SOURCEPOS_HPP_
#define _SOURCEPOS_HPP_

#include <cstdint>

namespace xylo
{
	struct SourcePos
	{
		uint64_t line;

		uint64_t col;
	};
}

#endif //_SOURCEPOS_HPP_
