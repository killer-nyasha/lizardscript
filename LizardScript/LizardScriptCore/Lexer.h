/*!
\file Lexer.h
\brief Lexical analyzer class
\author killer-nyasha
\version 0.2
\date 26.02.2020
*/
#pragma once
#include "SyntaxCore.h"
#include "i_lexer.h"
#include "algorithms.h"
#include "Pools.h"
#include "Logger.h"

namespace LizardScript
{
	class Lexer
	{
	private:

		//!special keyword which shows that next token is a string literal
		static KeywordToken stringKw;

		//!special keyword which shows that compiler have to interpret tokens within next branches as arguments passed to function
		static OperatorToken callKw;

		//!result of our work
		PoolPointer<LexerData> data;

		const SyntaxCore& core;

		//!source
		const TCHAR* text;

		size_t lastValueIndex = 0;
		//size_t textLength;

		//!when lexer meets first non-ascii symbol, it prints warning
		bool wasNonasciiWarningPrinted = false;

		//!type of the last keyword. it helps us to know which types of the next keyword are allowed 
		//!(for example binary operator cannot be met right after other binary operator)
		KeywordTokenType lastKeywordType = KeywordTokenType::Unary;

		//!alternative to isalnum. prints warning if symbol isn't from ASCII but doesn't throw an exception
		bool safe_isalnum(int ch)
		{
			if (ch > 255 || ch < -1)
			{
				if (!wasNonasciiWarningPrinted)
					logger.add("Not-ASCII symbols aren`t allowed (interpreted as whitespaces).");
				wasNonasciiWarningPrinted = true;
				return false;
			}
			else return isalnum(ch);
		}

		//!alternative to isgraph. prints warning if symbol isn't from ASCII but doesn't throw an exception
		bool safe_isgraph(int ch)
		{
			if (ch > 255 || ch < -1)
			{
				if (!wasNonasciiWarningPrinted)
					logger.add("Not-ASCII symbols aren`t allowed (interpreted as whitespaces).");
				wasNonasciiWarningPrinted = true;
				return false;
			}
			else return isgraph(ch);
		}

		bool charIsTextChar(TCHAR a)
		{
			return safe_isalnum(a) || (Search::binary(core.textChars, a) != -1);
		}

		void newToken();

		bool addFromList(const std::vector<KeywordToken*>& list, KeywordToken& pseudoKw);

		void init();

	public:

		Lexer(const SyntaxCore& c, const TCHAR* t);

		PoolPointer<LexerData> run();

	};
}