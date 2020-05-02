#pragma once
#include "LsFunction.h"

namespace LizardScript
{
	class Runtime
	{
	public:

		unsigned char stack[1024 * 10];

		void run(const LsFunction& f);

		//template <typename type>
		//auto _register(size_t esp, size_t i) { return reinterpret_cast<type*>(&stack[esp + i]); }
		
		//template <typename type>
		//auto _codeget(const LsFunction& f, size_t& eip) { return *(type*)(&f.code[(eip += sizeof(type)) - sizeof(type)]); }
	};
}