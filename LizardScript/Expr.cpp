#include "Expr.h"

using namespace LizardScript;

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
		DISASM(ret),

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

#define CODEGET(type) *(type*)(&code.data[(i += sizeof(type)) - sizeof(type)])

void Expr::disasm()
{
	for (size_t i = 0; i < code.data.size(); i += 2)
	{
		if ((opcode)code.data[i] == opcode::comment)
		{
			i += 2;
			int allocSize = CODEGET(int);

			std::string str = std::string(allocSize, ' ');

			for (size_t j = 0; j < allocSize; j++)
			{
				str[j] = CODEGET(char);
			}

			_out_ << "\t\t//" << str;
			i -= 2;
			_out_ << ENDL;
			continue;
		}

		if (disasmMap.count((opcode)code.data[i]) > 0)
		{
			auto command = disasmMap[(opcode)code.data[i]];
			_out_ << COLOR_YELLOW << i << ": " << "\t" << command << COLOR_NC << " ";
		}
		else
		{
			auto command = std::string("unknown") + std::to_string((int)code.data[i]);
			_out_ << COLOR_YELLOW << i << ": " << "\t" << command << COLOR_NC << " ";
		}

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
		else if ((opcode)code.data[i] == opcode::push_stringptr /*|| (opcode)code.data[i] == opcode::comment*/)
		{
			//_out_ << " offset: " << *reinterpret_cast<short int*>(&code.data[i + 2]);

			i += 2;
			int allocSize = CODEGET(int);

			std::string str = std::string(allocSize, ' ');

			for (size_t j = 0; j < allocSize; j++)
			{
				str[j] = CODEGET(char);
			}

			_out_ << " text: \"" << str << "\"";
			i -= 2;

			//i += *reinterpret_cast<int*>(&code.data[i + 2]) + 4;
		}

		_out_ << ENDL;

	}
	_out_ << COLOR_YELLOW << code.data.size() << ":\tend..." << ENDL;
}