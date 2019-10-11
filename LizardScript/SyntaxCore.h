#pragma once
#include "interpreter_util.h"
#include "Keyword.h"

namespace LizardScript
{
	//!syntax of expressions language
	class SyntaxCore
	{
	public:
		//!not-text chars which compiler reads as they were letters
		std::vector<TCHAR> textChars;

		//!Symbols which always split text into multiple tokens
		//!for example, (( - two tokens, unlike ++
		std::vector<TCHAR> breakChars;

		std::vector<Keyword> keywords;

		//!Table, which translates byte-code tofunction pointers
		//!created automatically in SyntaxCore constructor
		//KeywordAction* operatorsTable;

		SyntaxCore() 
		{ 
			sort();
		}

		void sort();

		inline bool isKeyword(TCHAR* token) const
		{
			return (Keyword*)token >= &keywords[0] && (Keyword*)token <= &keywords[keywords.size() - 1];
		}
	};

	extern SyntaxCore defaultSyntaxCore;
}