#pragma once
#include <vector>

using LsCode = unsigned char;
using LsInternalAddr = unsigned int;
using OFFSET_T = unsigned char;

class LsFunction
{
public:
	std::vector<unsigned char> code;

	void push_back(OFFSET_T value)
	{
		static_assert(sizeof(OFFSET_T) == 1, "push_back needs rewriting");
		code.push_back(value);
	}
};
