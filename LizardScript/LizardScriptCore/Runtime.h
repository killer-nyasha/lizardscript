#pragma once
#include "LsFunction.h"
#include "NonTypedStack2.h"

namespace LizardScript
{
	class Runtime
	{
	public:

		unsigned char* stack = new unsigned char[1024 * 10];

		void run(const LsFunction& f);

		template <typename T>
		T& getLocalSt(size_t index)
		{
			return *reinterpret_cast<T*>(&stack[index]);
		}

		template <typename T>
		void setLocalSt(size_t index, T& elem)
		{
			*reinterpret_cast<T*>(&stack[index]) = elem;
		}

		void setLocal(size_t index, const Dynamic& elem)
		{
			if (elem.byPtr())
				memcpy(&stack[index], *(void**)&elem.value, elem.type.size());
			else
				memcpy(&stack[index], (void*)&elem.value, elem.type.size());
		}
	};
}