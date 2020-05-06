#pragma once
#include <vector>

using LsCode = unsigned char;
using LsInternalAddr = unsigned int;

class LsFunction
{
public:
	std::vector<unsigned char> code;
};
