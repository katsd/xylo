//
// Created by Katsu Matsuda on 2020/04/22.
//

#include "VM/VM.hpp"

using namespace xylo::vm;

void VM::OutCode()
{
	uint64_t pc = 0;

	while (pc < code.size())
	{
		auto inst = code[pc];

		switch (inst)
		{
		case PUSH:
			printf("PUSH %llu\n", code[++pc]);
			break;

		case PUSH_TOP:
			printf("PUSH_TOP\n");
			break;

		case PUSH_OBJ:
			printf("PUSH_OBJ %llu\n", code[++pc]);
			break;

		case PUSH_GLOBAL_OBJ:
			printf("PUSH_GLOBAL_OBJ %llu\n", code[++pc]);
			break;

		case PUSH_CONST:
			printf("PUSH_CONST %llu\n", code[++pc]);
			break;

		case PUSH_ZERO:
			printf("PUSH_ZERO\n");
			break;

		case PUSH_ONE:
			printf("PUSH_ONE\n");
			break;

		case PUSH_START:
			printf("PUSH_START\n");
			break;

		case SET_OBJ:
			printf("SET_OBJ %llu\n", code[++pc]);
			break;

		case SET_GLOBAL_OBJ:
			printf("SET_GLOBAL_OBJ %llu\n", code[++pc]);
			break;

		case SET_OBJ_ZERO:
			printf("SET_OBJ_ZERO %llu\n", code[++pc]);
			break;

		case SET_OBJ_ONE:
			printf("SET_OBJ_ONE %llu\n", code[++pc]);
			break;

		case ICR_OBJ:
			printf("ICR_OBJ %llu\n", code[++pc]);
			break;

		case DCR_OBJ:
			printf("DCR_OBJ %llu\n", code[++pc]);
			break;

		case POP_TO_START:
			printf("POP_TO_START\n");
			break;

		case INPUT:
			printf("INPUT\n");
			break;

		case OUT:
			printf("OUT\n");
			break;

		case YAY:
			printf("YAY\n");
			break;

		case JUMP:
			printf("JUMP %llu\n", code[++pc]);
			break;

		case JUMP_TO_STACK_VALUE:
			printf("JUMP_TO_STACK_VALUE\n");
			break;

		case JUMP_IF:
			printf("JUMP_IF %llu\n", code[++pc]);
			break;

		case PUSH_OBJ_IDX_OFFSET:
			printf("PUSH_OBJ_IDX_OFFSET\n");
			break;

		case ADD_OBJ_IDX_OFFSET:
			printf("ADD_OBJ_IDX_OFFSET %llu\n", code[++pc]);
			break;

		case SET_OBJ_IDX_OFFSET:
			printf("SET_OBJ_IDX_OFFSET\n");
			break;

		case ICR_FUNC_LEVEL:
			printf("ICR_FUNC_LEVEL\n");
			break;

		case DCR_FUNC_LEVEL:
			printf("DCR_FUNC_LEVEL\n");
			break;

		case PUT_RETURN_VALUE:
			printf("PUT_RETURN_VALUE\n");
			break;

		case PUSH_RETURN_VALUE:
			printf("PUSH_RETURN_VALUE\n");
			break;

		case END:
			printf("END\n");
			break;

		case BOPE:
			printf("BOPE\n");
			break;

		case ADD:
			printf("+\n");
			break;

		case SUB:
			printf("-\n");
			break;

		case MUL:
			printf("*\n");
			break;

		case DIV:
			printf("/\n");
			break;

		case MOD:
			printf("%%\n");
			break;

		case BIN_AND:
			printf("&\n");
			break;

		case BIN_OR:
			printf("|\n");
			break;

		case BIN_XOR:
			printf("^\n");
			break;

		case EQUAL:
			printf("==\n");
			break;

		case NOT_EQUAL:
			printf("!=\n");
			break;

		case GREATER:
			printf(">\n");
			break;

		case GREATER_EQ:
			printf(">=\n");
			break;

		case LESS:
			printf("<\n");
			break;

		case LESS_EQ:
			printf("<=\n");
			break;

		case AND:
			printf("&&\n");
			break;

		case OR:
			printf("||\n");

		case MINUS:
			printf("- (unary)\n");
			break;

		case NOT:
			printf("!\n");
			break;

		case BIN_NOT:
			printf("~\n");
			break;

		default:
			printf("[Error] unknown instruction : %llu\n", inst);
			break;
		}
		pc++;
	}
}
