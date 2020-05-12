#pragma once
#include "LsFunction.h"

namespace LizardScript
{
	class Runtime
	{
	public:

		unsigned char* stack = new unsigned char[1024 * 10];

		void run(const LsFunction& f);

		template <typename T>
		T& getLocal(size_t index)
		{
			return *reinterpret_cast<T*>(&stack[index]);
		}

		template <typename T>
		void setLocal(size_t index, T& elem)
		{
			*reinterpret_cast<T*>(&stack[index]) = elem;
		}
	};
}