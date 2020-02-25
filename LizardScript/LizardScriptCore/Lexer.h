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

		bool wasNonasciiWarningPrinted = false;

		KeywordTokenType lastKeywordType = KeywordTokenType::Unary;

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

	public:

		void init();

		Lexer(const SyntaxCore& c, const TCHAR* t);

		PoolPointer<LexerData> run();

	};
}