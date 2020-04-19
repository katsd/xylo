//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _VALUE_HPP_
#define _VALUE_HPP_

#include <variant>
#include <memory>

#include "Int.hpp"
#include "Float.hpp"
#include "String.hpp"
#include "Variable.hpp"

namespace xylo::node
{
typedef std::variant<
	std::unique_ptr<Int>,
	std::unique_ptr<Float>,
	std::unique_ptr<String>,
	std::unique_ptr<Variable>
> Value;
}

#endif //_VALUE_HPP_


