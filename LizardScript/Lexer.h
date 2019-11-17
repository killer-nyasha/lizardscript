#pragma once
#include "SyntaxCore.h"

#include <cctype>

namespace LizardScript
{
	class Lexer
	{
	private:
		SyntaxCore& core;
		const TCHAR* text;
		size_t lastValueIndex;
		size_t textLength;
		std::vector<TCHAR>& values;
		std::vector<TCHAR*>& tokens;

		bool wasNonasciiWarningPrinted = false;

		bool safe_isalnum(int ch)
		{
			if (ch > 255 || ch < -1)
			{
				if (!wasNonasciiWarningPrinted) logger.add("Not-ASCII symbols aren`t allowed (interpreted as whitespaces).");
				wasNonasciiWarningPrinted = true;
				return false;
			}
			else return isalnum(ch);
		}

		bool safe_isgraph(int ch)
		{
			if (ch > 255 || ch < -1)
			{
				if (!wasNonasciiWarningPrinted) logger.add("Not-ASCII symbols aren`t allowed (interpreted as whitespaces).");
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

		void run();

	public:
		Lexer(SyntaxCore& c, const TCHAR* t, std::vector<TCHAR>& values, std::vector<TCHAR*>& tokens)
			: core(c), values(values), tokens(tokens)
		{
			text = t;
			textLength = _tcslen(text);
			//values.reserve(textLength + 64);
			lastValueIndex = 0;
			run();
		}

	};
}