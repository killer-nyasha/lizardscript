/*!
\file i_lexer.h
\brief Interface for calling lexical analyzer. 
Someday I'll make builder pattern for it, but now we don't have to use different lexers
\author killer-nyasha
\version 0.2
\date 26.02.2020
*/
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
		PoolPointer<std::vector<TCHAR>> values;
		PoolPointer<std::vector<void*>> tokens;

		size_t minReserved() const
		{ return 0; }
		size_t maxReserved() const
		{ return 65535; }

		//!\returns pointer to text of token at requested index
		//!\warning pointer is temporary - it will become invalid after deletion of LexerData instance or any modyfication of it
		TCHAR* text_at(size_t index)
		{
			size_t iIndex = reinterpret_cast<size_t>((*tokens)[index]);
			if (iIndex >= minReserved() && iIndex < maxReserved())
				return &(*values)[reinterpret_cast<size_t>((*tokens)[index])];
			else
				return reinterpret_cast<KeywordToken*>((*tokens)[index])->value;
		}

		//!\returns pointer to token at requested index
		//!\warning pointer is temporary - it will become invalid after deletion of LexerData instance or any modyfication of it
		void* operator[](size_t index)
		{
			size_t iIndex = reinterpret_cast<size_t>((*tokens)[index]);
			if (iIndex >= minReserved() && iIndex < maxReserved())
				return reinterpret_cast<void*>(&(*values)[reinterpret_cast<size_t>((*tokens)[index])]);
			else
				return reinterpret_cast<void*>((*tokens)[index]);
		}
	};

	//!Lexical analysis
	//!Splits text to tokens (such as operators, names and values)
	//!\returns pointer to LexerData object. you have to free it manually
	//!\param[in] core SyntaxCore
	//!\param[in] text Source code on LizardScript
	PoolPointer<LexerData> runLexer(const SyntaxCore& core, const TCHAR* text);
}