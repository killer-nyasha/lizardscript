#pragma once
#include "interpreter_util.h"
#include "TypeInfo.h"
#include "Registers.h"
#include "LsExpr.h"

namespace LizardScript
{
	using opcode = unsigned char;

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

		//ByteCode& operator<<(regindex value)
		//{
		//	return (*this) << regindex_pair(value);
		//}

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

		template <typename... P>
		void push(P... values)
		{
			int dummy[] = { (*this << values, 0)... };
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