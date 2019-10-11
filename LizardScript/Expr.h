#pragma once
#include "interpreter_util.h"
#include "TypeInfo.h"
#include "Registers.h"

#define DISASM(code) { opcode::code, #code }

namespace LizardScript
{
	//!byte-code type
	enum class opcode : unsigned char
	{
		nop = 0,
		set_32,
		set_64,

		push_32,
		push_64,
		push_this,
		push_offset,
		set_this,
		mov,
		get_32,
		get_64,
		push_stringptr,
		set_stringptr,

		push_stackptr,

		add_int_int,
		sub_int_int,
		mul_int_int,
		div_int_int,

		add_float_float,
		sub_float_float,
		mul_float_float,
		div_float_float,

		int_to_float,

		inc,
		dec,
		//_opt_push_this_and_offset,
		//_opt_set_this_and_offset,

		jmp,
		jz,

		more_int_int,
		less_int_int,

		call_cpp,

		alloc, 
		free,

		push_lsl = 0xff
	};

	extern std::map<opcode, const char*> disasmMap;

	class ByteCode
	{
	public:
		std::vector<unsigned char> data;

		ByteCode& operator<<(opcode value)
		{
			data.push_back(*reinterpret_cast<unsigned char*>(&value));
			return *this;
		}

		ByteCode& operator<<(regindex_pair value)
		{
			data.push_back(*reinterpret_cast<unsigned char*>(&value));
			return *this;
		}

		ByteCode& operator<<(regindex value)
		{
			return (*this) << regindex_pair(value);
		}

		ByteCode& operator<<(typed_reg value)
		{
			return (*this) << regindex_pair(value.index, value.index);
		}

		template <typename T>
		ByteCode& operator<<(T value)
		{
			if (data.capacity() < data.size() + sizeof(T))
				data.reserve((data.size() + sizeof(T)) * 12 / 10 + 8);
			T* ptr = reinterpret_cast<T*>(&data[data.size() - 1] + 1);
			data.resize(data.size() + sizeof(T));

			memcpy((void*)ptr, (void*)&value, sizeof(value));
			//*ptr = value;

			return *this;
		}
	};

	//!byte-code of expression
	//!use ExprCompiler to create object of this class
	class Expr
	{
	public:
		TypeInfo type;

		//!byte-code of expression
		ByteCode code;

		void optimize();

		void disasm();

		int maxStackSize = 0;

		//static std::vector<Keyword2>& initKeywords();
	};

	template <typename T>
	class TypedExpr : public Expr
	{
		void operator()(T& ths);
	};

}