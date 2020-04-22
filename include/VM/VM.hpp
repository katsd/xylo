//
// Created by Katsu Matsuda on 2020/04/21.
//

#ifndef _VM_HPP_
#define _VM_HPP_

#include <cstdint>
#include <vector>

#include "Inst.hpp"
#include "Obj.hpp"

namespace xylo::vm
{
class VM
{
 private:
	std::vector<uint64_t> code;

 public:
	void OutCode();

	VM(std::vector<uint64_t> code)
		: code(std::move(code))
	{

	}

};
}

#endif //_VM_HPP_
