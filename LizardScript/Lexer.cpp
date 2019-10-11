#include "Lexer.h"

using namespace LizardScript;

void Lexer::newToken()
{
	if (values.size() != lastValueIndex)
	{
		values.push_back(0);
		int kwi = vectorBinarySearch(core.keywords, Keyword(&(values)[lastValueIndex]));
		if (kwi != -1)
		{
			Keyword* kw = &core.keywords[kwi];
			tokens.push_back(kw->value);
			values.resize(lastValueIndex);
		}
		else
		{
			tokens.push_back(&values[lastValueIndex]);
			lastValueIndex = values.size();
		}
	}
}

void Lexer::run()
{
	size_t i = 0;
	while (i < textLength)
	{
		if (i + 1 < textLength && text[i] == '/' && text[i + 1] == '*')
		{
			int level = 1;
			i++;
			while (level != 0 && i + 1 < textLength)
			{
				if (text[i] == '/' && text[i + 1] == '*')
					level++;
				if (text[i] == '*' && text[i + 1] == '/')
					level--;
				i++;
			}
			i++;
		}
		else if (text[i] == '//')
			while (text[i] != '\r' && text[i] != '\n')
				i++;
		else if (text[i] == '[')
		{
			int level = 1;
			i++; newToken();

			values.push_back('$');
			newToken();

			while (level > 0 && i < textLength)
			{
				if (text[i] == '[')
					level++;
				if (text[i] == ']')
				{
					level--;
					if (level == 0) break;
				}
				values.push_back(text[i++]);
			}
			if (text[i] != ']')
			{
				throw Exception("Unclosed quote.");
			}
			i++; newToken();
		}
		else if (text[i] == '\"')
		{
			i++; newToken();

			values.push_back('$');
			newToken();

			while (text[i] != '\"' && i < textLength)
				values.push_back(text[i++]);
			if (text[i] != '\"')
			{
				throw Exception("Unclosed quote.");
			}
			i++; newToken();
		}
		else if (text[i] == '\`')
		{
			i++; newToken();

			while (text[i] != '\`' && i < textLength)
				values.push_back(text[i++]);
			if (text[i] != '\`')
			{
				throw Exception("Unclosed quote.");
			}
			i++; newToken();
		}
		else
		{
			if (safe_isgraph(text[i]))
			{
				values.push_back(text[i++]);

				if (vectorBinarySearch(core.breakChars, text[i]) != -1
					|| vectorBinarySearch(core.breakChars, text[i - 1]) != -1
					|| charIsTextChar(text[i - 1]) != charIsTextChar(text[i]))
					newToken();
			}
			else
			{
				i++; newToken();
			}
		}
	}
	newToken();
}