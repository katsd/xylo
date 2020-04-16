//
// Created by Katsu Matsuda on 2020/04/16.
//

#ifndef _NATIVE_HPP_
#define _NATIVE_HPP_

#include <vector>
#include <memory>
#include <string>

#include "vm/vm.hpp"

class Native
{
 private:
    struct Func
    {
        VM::Obj (* func)(std::unique_ptr<VM::Obj[]>& args, unsigned long arg_num);

        std::string func_name;

        unsigned long arg_num;
    };

    static std::vector<Func> funcs;

 public:
    static void Init();

    static void
    AddFunc(VM::Obj (* func)(std::unique_ptr<VM::Obj[]>& args, unsigned long arg_num), std::string name, unsigned long arg_num);

    static std::string StandardLibraryCode();

    static VM::Obj Call(unsigned long func_id, std::unique_ptr<VM::Obj[]>& args);

};

#endif //_NATIVE_HPP_
