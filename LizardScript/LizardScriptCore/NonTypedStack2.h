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

	template <typename T>
	T& get()
	{
		if (sizeof(T) <= sizeof(value))
			return *reinterpret_cast<T*>(value);
		else return **reinterpret_cast<T**>(value);
	}

	template <typename T>
	T*& pget()
	{
		return *reinterpret_cast<T**>(value);
	}
};



struct TempValue
{
	Dynamic d;
	bool compileTime;

	TempValue() { }

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
	PoolPointer<std::vector<TempValue>> data;

public:
	template <typename T>
	void push(const T& value)
	{
		data->push_back(TempValue(value));
	}

	TempValue pop()
	{
		TempValue t = data->back();
		data->pop_back();
		return t;
	}

	size_t size() const
	{
		return data->size();
	}

	TempValue& operator[](size_t i)
	{
		return (*data)[i];
	}

	void resize(size_t sz)
	{
		data->resize(sz);
	}
};