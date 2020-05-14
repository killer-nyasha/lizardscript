/*!
\file NonTypedStack.h
\brief Stores elements with different sizes and types.
\author killer-nyasha
\version 0.2
\date 23.00.2020
*/
#pragma once
#include <stack>

#include "interpreter_util.h"
#include "TypeInfo.h"
#include "LsTypedefs.h"

#include "Pools.h"

//!All genius is simple ^_^
struct Dynamic
{
	TypeInfo type;
	char value[16];

	Dynamic() { }

	template <typename T>
	Dynamic(const T& arg)
	{
		if (sizeof(arg) <= sizeof(value))
		{
			memcpy(&value, &arg, sizeof(arg));
		}
		else
		{
			T* pArg = new T(std::move(arg));
			memcpy(&value, &pArg, sizeof(pArg));
		}
		type = typeInfo<T>();
	}

	bool byPtr() const
	{
		return type.size() > sizeof(value);
	}
};



struct TempValue
{
	Dynamic d;
	bool compileTime;

	TempValue(const Dynamic& d, bool compileTime = true) 
		: d(d), compileTime(compileTime)
	{

	}
};

struct LocalVariable
{
	const TCHAR* name;
	Dynamic variable;
};

class NonTypedStack2
{
private:
	PoolPointer<std::stack<TempValue>> data;

public:
	template <typename T>
	void push(const T& value)
	{
		data->push(TempValue(value));
	}

	TempValue pop()
	{
		TempValue t = data->top();
		data->pop();
		return t;
	}

	size_t size() const
	{
		return data->size();
	}
};