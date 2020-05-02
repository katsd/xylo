//
// Created by Katsu Matsuda on 2020/04/27.
//

#ifndef _NATIVE_HPP_
#define _NATIVE_HPP_

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "vm/vm.hpp"
#include "cxylo.h"

namespace xylo::native
{
class Native
{
 public:
	struct Func;

 private:
	static std::vector<Func> funcs;

 public:
	struct Func
	{
		vm::Obj (* func)(std::unique_ptr<vm::Obj[]>& args, uint64_t arg_num);

		//CObj (* ext_func)(CObj args[], unsigned long arg_num);

		std::string func_name;

		uint64_t arg_num;

		bool is_external;
	};

	static void DeleteAllFunc();

	static void Init();

	static void AddFunc(const Func& func);

	static void
	AddFunc(vm::Obj (* func)(std::unique_ptr<vm::Obj[]>& args, uint64_t arg_num), std::string name, uint64_t arg_num);

	static void AddFunc(std::string name, unsigned long arg_num);

	static std::string StandardLibraryCode();

	static vm::Obj Call(uint64_t func_id, std::unique_ptr<vm::Obj[]>& args, const void* ext_xylo_instance);
};
}

#endif //_NATIVE_HPP_
