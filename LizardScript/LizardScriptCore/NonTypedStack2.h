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

//!All genius is simple ^_^
struct Dynamic
{
	TypeInfo type;
	int64 value;

	Dynamic() { }
};

template <typename T>
Dynamic makeDynamic(const T& arg)
{
	Dynamic dyn;
	static_assert(sizeof(arg) <= sizeof(dyn.value), "Argument is too big, it must be in the heap");
	memcpy(&dyn.value, &arg, sizeof(arg));
	dyn.type = typeInfo<T>();
	return dyn;
}

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
	TCHAR* name;
	Dynamic variable;
};

class NonTypedStack2
{
private:
	std::stack<TempValue> data;

public:
	template <typename T>
	void push(const T& value)
	{
		data.push(TempValue(makeDynamic(value)));
	}

	TempValue pop()
	{
		TempValue t = data.top();
		data.pop();
		return t;
	}
};