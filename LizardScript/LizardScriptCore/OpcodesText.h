#pragma once
#include <map>

#include "LsFunction.h"

struct cmp_str
{
	bool operator()(char const* a, char const* b) const
	{
		return std::strcmp(a, b) < 0;
	}
};

extern std::map<const char*, LsCode, cmp_str> textToOpcode;
extern const char* opcodeToText[];
extern bool opcodes_initialized;

inline void fillOpcodeToText()
{
	if (!opcodes_initialized)
	{
		opcodes_initialized = true;
		for (auto& pair : textToOpcode)
			opcodeToText[pair.second] = pair.first;
	}
}
