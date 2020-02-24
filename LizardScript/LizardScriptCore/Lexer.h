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

		static KeywordToken stringKw;
		static OperatorToken callKw;

		//!result of our work
		PoolPointer<LexerData> data;

		SyntaxCore& core;

		//!source
		const TCHAR* text;

		size_t lastValueIndex = 0;
		size_t textLength;

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
			return safe_isalnum(a) || (binarySearch(core.textChars, a) != -1);
		}

		void newToken();

		bool addFromList(const std::vector<KeywordToken*>& list, KeywordToken& pseudoKw);

	public:

		void init();

		Lexer(SyntaxCore& c, const TCHAR* t);

		PoolPointer<LexerData> run();

	};
}