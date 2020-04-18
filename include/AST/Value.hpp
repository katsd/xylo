//
// Created by Katsu Matsuda on 2020/04/18.
//

#ifndef _VALUE_HPP_
#define _VALUE_HPP_

#include <variant>

#include "Int.hpp"
#include "Float.hpp"
#include "String.hpp"
#include "Variable.hpp"

namespace xylo::node
{
typedef std::variant<Int, Float, String, Variable> Value;
}

#endif //_VALUE_HPP_


