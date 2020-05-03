#pragma once
#include <map>

extern std::map<const char*, int> textToOpcode;
extern const char* opcodeToText[];

inline void fillOpcodeToText()
{
	for (auto& pair : textToOpcode)
	{
		opcodeToText[pair.second] = pair.first;
	}
}