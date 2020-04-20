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
	uint64_t line = 0;

	uint64_t col = 0;
};
}

#endif //_SOURCEPOS_HPP_
