#pragma once
#include "SyntaxCore.h"
#include "ILexer.h"

namespace LizardScript
{
	class Lexer
	{
	private:
		LexerData* data;
		bool isMultiThread;

		SyntaxCore& core;
		const TCHAR* text;
		size_t lastValueIndex;
		size_t textLength;

		bool wasNonasciiWarningPrinted = false;

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
			return safe_isalnum(a) || (vectorBinarySearch(core.textChars, a) != -1);
		}

		void newToken();

	public:
		Lexer(SyntaxCore& c, const TCHAR* t, bool multiThread);


		virtual ~Lexer()
		{
			if (isMultiThread)
				delete data;
		}

		LexerData* run();

	};
}