//
// Created by Katsu Matsuda on 2020/04/21.
//

#ifndef _VM_HPP_
#define _VM_HPP_

#include <iostream>
#include <cstdint>
#include <memory>
#include <mutex>
#include <vector>

#include "Inst.hpp"
#include "Obj.hpp"

namespace xylo::vm
{
class VM
{
 private:
	static constexpr uint64_t global_obj_table_size = 1024 * 128;

	static constexpr uint64_t obj_table_size = 1024 * 128;

	static constexpr uint64_t stack_size = 1024 * 1024;

	const std::vector<uint64_t> code;

	const std::vector<Obj> const_table;

	std::mutex global_obj_table_mutex;

	std::unique_ptr<Obj[]> global_obj_table;

	inline Obj& GetStack(uint64_t& sc, const std::unique_ptr<Obj[]>& stack)
	{
		return stack[--sc];
	}

	inline void PushStack(uint64_t& sc, const std::unique_ptr<Obj[]>& stack, const Obj& obj)
	{
		stack[sc++] = obj;
	}

 public:
	Obj Run(uint64_t start_idx = 0);

	void OutCode();

	VM(const std::vector<uint64_t> code, const std::vector<Obj> const_table)
		: code(std::move(code)), const_table(std::move(const_table))
	{
		global_obj_table = std::make_unique<Obj[]>(global_obj_table_size);
	}

};
}

#endif //_VM_HPP_
