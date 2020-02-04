#pragma once
#include "interpreter_util.h"

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
	//!\param[in] text expression source text
	//void runLexer(const TCHAR* t, std::vector<TCHAR>& values, std::vector<TCHAR*>& tokens);
	PoolPointer<LexerData> runLexer(SyntaxCore& core, const TCHAR* text);
}