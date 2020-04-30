//
// Created by Katsu Matsuda on 2020/04/21.
//

#include "vm/vm.hpp"
#include "native/native.hpp"

using namespace xylo::vm;

Obj VM::Run(const std::string& func_name, const std::vector<Obj>& args)
{
	std::string key = func_name + "_" + std::to_string(args.size());

	auto pair = func_start_idx.find(key);

	if (pair == func_start_idx.end())
		return Obj{};

	return Run((*pair).second, args);
}

Obj VM::Run(uint64_t start_idx, const std::vector<Obj>& args)
{
	uint64_t pc = start_idx;

	if (code[pc] != START)
	{
		printf("failed to run\n");
		return Obj{};
	}

	uint64_t sc = 0;

	uint64_t obj_idx_offset = 0;

	uint64_t func_level = 0;

	std::unique_ptr<Obj[]> obj_table(new Obj[obj_table_size]);

	std::unique_ptr<Obj[]> stack(new Obj[stack_size]);

	std::unique_ptr<Obj> return_value = nullptr;

	for (uint64_t i = args.size() - 1; i >= 0; i--)
		PushStack(sc, stack, args[i]);

	while (pc < code.size())
	{
		auto inst = code[pc];

		switch (inst)
		{
		case PUSH:
			PushStack(sc, stack, Obj{ (int64_t)code[++pc] });
			break;

		case PUSH_TOP:
			PushStack(sc, stack, GetStack(sc, stack));
			break;

		case PUSH_OBJ:
			PushStack(sc, stack, obj_table[code[++pc] + obj_idx_offset]);
			break;

		case PUSH_GLOBAL_OBJ:
		{
			std::lock_guard<std::mutex> lock(global_obj_table_mutex);
			PushStack(sc, stack, global_obj_table[code[++pc]]);
		}
			break;

		case PUSH_CONST:
			PushStack(sc, stack, const_table[code[++pc]]);
			break;

		case PUSH_ZERO:
			PushStack(sc, stack, Obj{ (int64_t)0 });
			break;

		case PUSH_ONE:
			PushStack(sc, stack, Obj{ (int64_t)1 });
			break;

		case PUSH_START:
			PushStack(sc, stack, Obj{ Inst::START });
			break;

		case SET_OBJ:
			obj_table[code[++pc] + obj_idx_offset] = GetStack(sc, stack);
			break;

		case SET_GLOBAL_OBJ:
		{
			std::lock_guard<std::mutex> lock(global_obj_table_mutex);
			global_obj_table[code[++pc]] = GetStack(sc, stack);
		}
			break;

		case SET_OBJ_ZERO:
			obj_table[code[++pc] + obj_idx_offset] = Obj{ (int64_t)0 };
			break;

		case SET_OBJ_ONE:
			obj_table[code[++pc] + obj_idx_offset] = Obj{ (int64_t)1 };
			break;

		case ICR_OBJ:
		{
			auto obj = obj_table[code[++pc] + obj_idx_offset];
			obj_table[code[pc] + obj_idx_offset].Set(obj.GetInt() + 1);
		}
			break;

		case DCR_OBJ:
		{
			auto obj = obj_table[code[++pc] + obj_idx_offset];
			obj_table[code[pc] + obj_idx_offset].Set(obj.GetInt() - 1);
		}
			break;

		case POP_TO_START:
			while (sc >= 0)
			{
				if (GetStack(sc, stack).GetInst() == Inst::START)
					break;
			}
			break;

		case INPUT:
		{
			std::string str;
			std::cin >> str;

			PushStack(sc, stack, Obj{ str });
		}
			break;

		case OUT:
			printf("%s\n", GetStack(sc, stack).GetString().c_str());
			break;

		case YAY:
			printf("Yay\n");
			break;

		case JUMP:
			pc = code[++pc];
			continue;

		case JUMP_TO_STACK_VALUE:
			pc = GetStack(sc, stack).GetInt();
			continue;

		case JUMP_IF:
			if (GetStack(sc, stack).GetInt() != 0)
			{
				pc = code[++pc];
				continue;
			}
			pc++;
			break;

		case PUSH_OBJ_IDX_OFFSET:
			PushStack(sc, stack, Obj{ (int64_t)obj_idx_offset });
			break;

		case ADD_OBJ_IDX_OFFSET:
			obj_idx_offset += code[++pc];
			break;

		case SET_OBJ_IDX_OFFSET:
			obj_idx_offset = GetStack(sc, stack).GetInt();
			break;

		case ICR_FUNC_LEVEL:
			func_level++;
			break;

		case DCR_FUNC_LEVEL:
			func_level--;
			break;

		case PUT_RETURN_VALUE:
			return_value = std::make_unique<Obj>(GetStack(sc, stack));
			break;

		case PUSH_RETURN_VALUE:
			PushStack(sc, stack, *return_value);
			return_value = nullptr;
			break;

		case CALL_NATIVE:
		{
			auto func_id = code[++pc];
			auto arg_num = code[++pc];

			auto args = std::make_unique<Obj[]>(arg_num);

			for (int i = 0; i < arg_num; i++)
				args[i] = GetStack(sc, stack);

			auto res = native::Native::Call(func_id, args);

			PushStack(sc, stack, res);
		}
			break;

		case START:
			break;

		case END:
			if (func_level == 0)
				return GetStack(sc, stack);
			break;

		case BOPE:
		{
			auto ope = code[++pc];

			auto left = GetStack(sc, stack);
			auto right = GetStack(sc, stack);

			bool is_int = left.GetType() == ObjType::INT && right.GetType() == ObjType::INT;

			Obj obj;

			switch (ope)
			{
			case ADD:
				if (is_int)
					obj.Set(left.GetInt() + right.GetInt());
				else
					obj.Set(left.GetFloat() + right.GetFloat());
				break;

			case SUB:
				if (is_int)
					obj.Set(left.GetInt() - right.GetInt());
				else
					obj.Set(left.GetFloat() - right.GetFloat());
				break;

			case MUL:
				if (is_int)
					obj.Set(left.GetInt() * right.GetInt());
				else
					obj.Set(left.GetFloat() * right.GetFloat());
				break;

			case DIV:
				if (is_int)
					obj.Set(left.GetInt() / right.GetInt());
				else
					obj.Set(left.GetFloat() / right.GetFloat());
				break;

			case MOD:
				obj.Set(left.GetInt() % right.GetInt());
				break;

			case BIN_AND:
				obj.Set(left.GetInt() & right.GetInt());
				break;

			case BIN_OR:
				obj.Set(left.GetInt() | right.GetInt());
				break;

			case BIN_XOR:
				obj.Set(left.GetInt() ^ right.GetInt());

				break;

			case EQUAL:
				obj.Set((int64_t)(left == right ? 1 : 0));
				break;

			case NOT_EQUAL:
				obj.Set((int64_t)(left != right ? 1 : 0));
				break;

			case GREATER:
				if (is_int)
					obj.Set((int64_t)(left.GetInt() > right.GetInt() ? 1 : 0));
				else
					obj.Set((int64_t)(left.GetFloat() > right.GetFloat() ? 1 : 0));
				break;

			case GREATER_EQ:
				if (is_int)
					obj.Set((int64_t)(left.GetInt() >= right.GetInt() ? 1 : 0));
				else
					obj.Set((int64_t)(left.GetFloat() >= right.GetFloat() ? 1 : 0));
				break;

			case LESS:
				if (is_int)
					obj.Set((int64_t)(left.GetInt() < right.GetInt() ? 1 : 0));
				else
					obj.Set((int64_t)(left.GetFloat() < right.GetFloat() ? 1 : 0));
				break;

			case LESS_EQ:
				if (is_int)
					obj.Set((int64_t)(left.GetInt() <= right.GetInt() ? 1 : 0));
				else
					obj.Set((int64_t)(left.GetFloat() <= right.GetFloat() ? 1 : 0));
				break;

			case AND:
				obj.Set((int64_t)((left.GetInt() != 0 && right.GetInt()) != 0 ? 1 : 0));
				break;

			case OR:
				obj.Set((int64_t)((left.GetInt() != 0 || right.GetInt()) != 0 ? 1 : 0));
				break;

			default:
				break;
			}

			PushStack(sc, stack, obj);
		}
			break;

		case MINUS:
		{
			auto obj = GetStack(sc, stack);
			if (obj.GetType() == ObjType::INT)
				PushStack(sc, stack, Obj{ -obj.GetInt() });
			else
				PushStack(sc, stack, Obj{ -obj.GetFloat() });
		}
			break;

		case NOT:
			PushStack(sc, stack, Obj{ (int64_t)(GetStack(sc, stack).GetInt() == 0 ? 1 : 0) });
			break;

		case BIN_NOT:
			PushStack(sc, stack, Obj{ (int64_t)(~GetStack(sc, stack).GetInt()) });
			break;

		default:
			printf("[Error] unknown instruction : %llu\n", inst);
			break;
		}
		pc++;
	}

	return Obj{};
}