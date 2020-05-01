//
// Created by Katsu Matsuda on 2020/04/21.
//

#ifndef _VM_HPP_
#define _VM_HPP_

#include <iostream>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "inst.hpp"
#include "obj.hpp"

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

	const std::map<std::string, uint64_t> func_start_idx;

	std::mutex global_obj_table_mutex;

	std::unique_ptr<Obj[]> global_obj_table;

	const void* ext_xylo_instance;

	inline Obj& GetStack(uint64_t& sc, const std::unique_ptr<Obj[]>& stack)
	{
		if (sc >= stack_size)
		{
			puts("Stack Overflow");
			exit(0);
		}

		return stack[--sc];
	}

	inline void PushStack(uint64_t& sc, const std::unique_ptr<Obj[]>& stack, const Obj& obj)
	{
		stack[sc++] = obj;
	}

 public:
	Obj Run(uint64_t start_idx = 0, const std::vector<Obj>& args = {});

	Obj Run(const std::string& func_name, const std::vector<Obj>& args);

	void OutCode();

	VM(std::vector<uint64_t> code,
		std::vector<Obj> const_table,
		std::map<std::string, uint64_t> func_start_idx,
		const void* ext_xylo_instance)
		: code(std::move(code)), const_table(std::move(const_table)), func_start_idx(std::move(func_start_idx)), ext_xylo_instance(ext_xylo_instance)
	{
		global_obj_table = std::make_unique<Obj[]>(global_obj_table_size);
	}

};
}

#endif //_VM_HPP_
