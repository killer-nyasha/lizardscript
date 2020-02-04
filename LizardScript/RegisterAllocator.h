#pragma once
#include "ByteCodeGenerator.h"
#include "Pools.h"

template <size_t size>
class RegisterAllocator
{
public:
	PoolPointer<std::stack<typed_reg>> stackEmul;
	PoolPointer<std::stack<regindex>> freeReg;

	RegisterAllocator()
	{
		while (!stackEmul->empty())
			stackEmul->pop();
		while (!freeReg->empty())
			freeReg->pop();

		for (regindex t = size - 1; t + 1 > 1; t--)
			freeReg->push(t);
	}

	typed_reg& alloc(TypeInfo type)
	{
		if (freeReg->size() == 0)
			throw Exception("Not enough free registers.");

		stackEmul->push(typed_reg(freeReg->top(), type));
		freeReg->pop();
		return stackEmul->top();
	}

	//возможно, забагованная
	typed_reg& push(typed_reg reg)
	{
		stackEmul->push(reg);
		freeReg->pop();
		return stackEmul->top();
	}

	typed_reg free()
	{
		if (stackEmul->size() == 0)
			throw Exception("Not enough operands.");

		typed_reg r = stackEmul->top();
		freeReg->push(r.index);
		stackEmul->pop();
		return r;
	}

	void clear()
	{
		while (stackEmul->size() > 0)
		{
			//typed_reg r = stackEmul->top();
			freeReg->push(stackEmul->top().index);
			stackEmul->pop();
		}
	}
};
