#pragma once
#include <vector>

#include "crossplatform_tchar.h"
#include "Pools.h"

namespace LizardScript
{
	class SyntaxCore;

	struct LexerData
	{
		std::vector<TCHAR> values;
		std::vector<TCHAR*> tokens;
	};

	//!Lexical analysis
	//!Splits text to tokens (such as operators, names and values)
	//!\returns pointer to LexerData object. you have to free it manually
	//!\param[in] core SyntaxCore
	//!\param[in] text Source code on LizardScript
	PoolPointer<LexerData> runLexer(const SyntaxCore& core, const TCHAR* text);
}