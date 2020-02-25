#pragma once
#include <vector>

#include "SyntaxCore.h"//?
#include "crossplatform_tchar.h"
#include "Pools.h"

namespace LizardScript
{
	//class SyntaxCore;

	struct LexerData
	{
		std::vector<TCHAR> values;
		std::vector<void*> tokens;

		size_t minReserved()
		{ return 0; }
		size_t maxReserved()
		{ return 65535; }

		//!\returns pointer to token at requested index
		//!\warning pointer is temporary - it will become invalid after deletion of LexerData instance or any modyfication of it
		TCHAR* operator[](size_t index)
		{
			size_t iIndex = reinterpret_cast<size_t>(tokens[index]);
			if (iIndex >= minReserved() && iIndex < maxReserved())
				return &values[reinterpret_cast<size_t>(tokens[index])]; 
			else
				return reinterpret_cast<TCHAR*>(tokens[index]);
		}
	};

	//!Lexical analysis
	//!Splits text to tokens (such as operators, names and values)
	//!\returns pointer to LexerData object. you have to free it manually
	//!\param[in] core SyntaxCore
	//!\param[in] text Source code on LizardScript
	PoolPointer<LexerData> runLexer(const SyntaxCore& core, const TCHAR* text);
}