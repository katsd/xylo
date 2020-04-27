//
// Created by Katsu Matsuda on 2020/04/27.
//

#ifndef _NATIVE_HPP_
#define _NATIVE_HPP_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "VM/VM.hpp"

namespace xylo::native
{
class Native
{
 private:
	struct Func
	{
		vm::Obj (* func)(std::unique_ptr<vm::Obj[]>& args, uint64_t arg_num);

		std::string func_name;

		uint64_t arg_num;
	};

	static std::vector<Func> funcs;

 public:
	static void Init();

	static void
	AddFunc(vm::Obj (* func)(std::unique_ptr<vm::Obj[]>& args, uint64_t arg_num), std::string name, uint64_t arg_num);

	static std::string StandardLibraryCode();

	static vm::Obj Call(uint64_t func_id, std::unique_ptr<vm::Obj[]>& args);
};
}

#endif //_NATIVE_HPP_
