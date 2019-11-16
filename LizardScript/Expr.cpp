#include "Expr.h"

using namespace LizardScript;

//void ByteCodeExpr::optimize()
//{
//	vector_rewrite<opcode> ncode(code);
//
//	for (size_t i = 0; i < code.size(); i += 2)
//	{
//		if (code[i] == opcode::push_32 && code[i + 2] == opcode::add_int_int && *reinterpret_cast<int*>(&code[i + 4]) == 1)
//		{
//			ncode.push_back(opcode::inc);
//			ncode.push_back(code[i + 3]);
//			i += 6;
//			continue;
//		}
//		else if (i + 3 < code.size() && code[i + 1] == code[i + 3])
//		{
//			if (code[i] == opcode::push_this && code[i + 2] == opcode::push_offset)
//			{
//				ncode.push_back(opcode::_opt_push_this_and_offset);
//				ncode.push_back(code[i + 1]);
//
//				ncode.push_back(code[i + 4]);
//				ncode.push_back(code[i + 5]);
//				i += 4;
//				continue;
//			}
//			else if (code[i] == opcode::set_this && code[i + 2] == opcode::push_offset)
//			{
//				ncode.push_back(opcode::_opt_set_this_and_offset);
//				ncode.push_back(code[i + 1]);
//
//				ncode.push_back(code[i + 4]);
//				ncode.push_back(code[i + 5]);
//				i += 4;
//				continue;
//			}
//		}
//		{
//			ncode.push_back(code[i]);
//			ncode.push_back(code[i + 1]);
//
//			if (code[i] == opcode::push_32)
//			{
//				ncode.push_back(code[i + 2]);
//				ncode.push_back(code[i + 3]);
//				ncode.push_back(code[i + 4]);
//				ncode.push_back(code[i + 5]);
//				i += 4;
//			}
//			else if (code[i] == opcode::push_offset)
//			{
//				ncode.push_back(code[i + 2]);
//				ncode.push_back(code[i + 3]);
//				i += 2;
//			}
//		}
//	}
//
//	//code = std::move(ncode);
//	code.resize(ncode.size);
//}

//
//есть не все команды!!!

namespace LizardScript
{
	std::map<opcode, const char*> disasmMap =
	{
		DISASM(nop),
		DISASM(sub_float_float),
		DISASM(div_float_float),
		DISASM(sub_int_int),
		DISASM(div_int_int),

		DISASM(push_stringptr),

		DISASM(int_to_float),

		DISASM(more_int_int),
		DISASM(less_int_int),

		DISASM(alloc),
		DISASM(free),

		DISASM(set_32),
		DISASM(set_64),
		DISASM(set_big),

		DISASM(push_32),
		DISASM(push_this),
		DISASM(push_offset),
		DISASM(set_this),
		DISASM(mov),
		DISASM(get_32),
		DISASM(get_64),
		DISASM(add_int_int),
		DISASM(mul_int_int),
		DISASM(add_float_float),
		DISASM(mul_float_float),
		DISASM(inc),
		DISASM(dec),
		DISASM(jmp),
		DISASM(jz),

		DISASM(push_lsl),
		DISASM(push_stackbase),
		DISASM(push_stackptr),
		DISASM(set_stackptr),

		DISASM(call_cpp),
		DISASM(set_stringptr),
	};
}

using namespace LizardScript;

#include <iostream>

#define _out_ logger

void Expr::disasm()
{
	for (size_t i = 0; i < code.data.size(); i += 2)
	{
		auto command = disasmMap.count((opcode)code.data[i]) > 0 ? disasmMap[(opcode)code.data[i]] : "unknown";

		_out_ << COLOR_YELLOW << i << ": " << "\t" << command << COLOR_NC << " ";
		operator<<(_out_, *reinterpret_cast<regindex_pair*>(&code.data[i + 1]));

		if ((opcode)code.data[i] == opcode::push_32 || (opcode)code.data[i] == opcode::jz || (opcode)code.data[i] == opcode::jmp)
		{
			_out_ << " value: " << *reinterpret_cast<int*>(&code.data[i + 2]);
			i += 4;
		}
		else if ((opcode)code.data[i] == opcode::alloc || (opcode)code.data[i] == opcode::push_offset 
			|| (opcode)code.data[i] == opcode::set_big || (opcode)code.data[i] == opcode::set_stackptr
			/*|| (opcode)code.data[i] == opcode::_opt_push_this_and_offset |
			| (opcode)code.data[i] == opcode::_opt_set_this_and_offset*/
			)
		{
			_out_ << " offset: " << *reinterpret_cast<short int*>(&code.data[i + 2]);
			i += 2;
		}
		else if ((opcode)code.data[i] == opcode::call_cpp)
		{
			i += sizeof(FunctionInfo().callStruct);
		}
		else if ((opcode)code.data[i] == opcode::push_stringptr)
		{
			i += *reinterpret_cast<int*>(&code.data[i + 2]) + 4;
		}

		_out_ << ENDL;

	}
	_out_ << COLOR_YELLOW << code.data.size() << ":\tend..." << ENDL;
}