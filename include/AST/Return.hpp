//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _RETURN_HPP_
#define _RETURN_HPP_

#include <memory>

#include "Exp.hpp"
#include "Token/SourcePos.hpp"

namespace xylo::node
{
struct Return
{
	std::unique_ptr<Exp> exp;

	SourcePos pos;
};
}

#endif //_RETURN_HPP_
