#pragma once
#include <vector>

#include "crossplatform_tchar.h"

using LsCode = unsigned char;
using LsInternalAddr = unsigned int;
using OFFSET_T = unsigned char;

class LsFunction
{
public:
	std::vector<unsigned char> code;

	template <typename T>
	void push_back(const T& value)
	{
		//static_assert(sizeof(T) == 1, "push_back needs rewriting");
		
		for (size_t i = 0; i < sizeof(value); i++)
			code.push_back(((char*)&value)[i]);
	}

	void push_str(TCHAR* value)
	{
		size_t length = _tcslen(value) + 1;
		for (size_t i = 0; i < length; i++)
			code.push_back(value[i]);
	}
};
