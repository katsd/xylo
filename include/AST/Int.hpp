//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _INT_HPP_
#define _INT_HPP_

#include <cstdint>
#include <string>

#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Int
{
	uint64_t value;

	SourcePos pos;
};
}

#endif //_INT_HPP_
