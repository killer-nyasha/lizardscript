#include "pch.h"
#include "Lexer.h"
#include "algorithms.h"
#include "Tokens.h"
#include "interpreter_util.h"
#include "SyntaxCore.h"
#include "Exception.h"

#include <iostream>

using namespace LizardScript;

KeywordToken Lexer::stringKw = KeywordToken("__string__");
OperatorToken Lexer::callKw = OperatorToken("__call__", KeywordTokenType::Simple, 100);

void Lexer::init()
{
	data->values->resize(0);
	data->tokens->resize(0);
}

Lexer::Lexer(const SyntaxCore& c, const TCHAR* t, size_t length)
	: core(c), text(t)
{
	init();

	if (length != 0)
		textLength = length;
	else textLength = _tcslen(text);

	//std::cout << length << "\t" << text;
	//std::cout << "\n\n";
	//for (size_t i = 0; i < textLength; i++)
	//	std::cout << text[i];
	//std::cout << "\n\n";
}

void Lexer::newToken()
{
	ALIAS(data->values, values);
	ALIAS(data->tokens, tokens);

	if (values->size() != lastValueIndex)
		//if we add something new
	{
		values->push_back(0);//null-terminator for current token
		TCHAR* kwtext = &(*values)[lastValueIndex];

		//std::cout << "...\t" << kwtext << "\n";

		//if (charIsTextChar((*values)[lastValueIndex]))
		{
			//try to find a simple keyword
			int index = pSearch::binary(core.simpleKeywords, &kwtext);
			if (index != -1)
			{
				KeywordToken* kw = &*core.simpleKeywords[index];
				tokens->push_back(reinterpret_cast<void*>(kw));
				values->resize(lastValueIndex);

				if (kwtext[0] == '(')
				{
					hasSpacesPost = true;
					if (lastKeywordType == KeywordTokenType::Simple)
					{
						tokens->push_back(reinterpret_cast<void*>(&callKw));
					}
				}
				else if (kwtext[0] == ')')
					hasSpacesPost = false;

				lastKeywordType = kw->type;
				hasSpacesPre = hasSpacesPost;
				return;
			}
		}

		if (hasSpacesPost == hasSpacesPre)
		{
			//binary
			int index = pSearch::binary(core.binaryOperators, &kwtext);
			if (index != -1)
			{
				KeywordToken* kw = &*core.binaryOperators[index];
				tokens->push_back(reinterpret_cast<void*>(kw));
				values->resize(lastValueIndex);
				lastKeywordType = kw->type;
				hasSpacesPre = hasSpacesPost;
				return;
			}
		}
		else if (!hasSpacesPost && hasSpacesPre)
		{
			//prefix
			int index = pSearch::binary(core.prefixUnary, &kwtext);
			if (index != -1)
			{
				KeywordToken* kw = &*core.prefixUnary[index];
				tokens->push_back(reinterpret_cast<void*>(kw));
				values->resize(lastValueIndex);
				lastKeywordType = kw->type;
				hasSpacesPre = hasSpacesPost;
				return;
			}
		}
		else if (hasSpacesPost && !hasSpacesPre)
		{
			//postfix
			int index = pSearch::binary(core.postfixUnary, &kwtext);
			if (index != -1)
			{
				KeywordToken* kw = &*core.postfixUnary[index];
				tokens->push_back(reinterpret_cast<void*>(kw));
				values->resize(lastValueIndex);
				lastKeywordType = kw->type;
				hasSpacesPre = hasSpacesPost;
				return;
			}
		}

		//identifier
		tokens->push_back(reinterpret_cast<void*>(lastValueIndex));
		lastValueIndex = values->size();
		lastKeywordType = KeywordTokenType::Simple;
		hasSpacesPre = hasSpacesPost;
	}
	hasSpacesPre = hasSpacesPost;
}

void Lexer::processComments(size_t& i)
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
	else if (i + 1 < textLength && text[i] == '/' && text[i + 1] == '/')
		while (text[i] != '\r' && text[i] != '\n')
			i++;
}

void Lexer::processQuotes(size_t& i)
{
	ALIAS(data->values, values);
	ALIAS(data->tokens, tokens);

	if (text[i] == '[')
	{
		int level = 1;
		i++; newToken();

		tokens->push_back(reinterpret_cast<void*>(&stringKw));

		while (level > 0 && i < textLength)
		{
			if (text[i] == '[')
				level++;
			if (text[i] == ']')
			{
				level--;
				if (level == 0) break;
			}
			values->push_back(text[i++]);
		}
		if (text[i] != ']')
		{
			throw Exception("Quote wasn't closed");
		}
		i++; newToken();
	}
	else if (text[i] == '\"')
	{
		i++; newToken();

		values->push_back('$');
		newToken();

		while (text[i] != '\"' && i < textLength)
			values->push_back(text[i++]);
		if (text[i] != '\"')
		{
			throw Exception("Quote wasn't closed");
		}
		i++; newToken();
	}
}

PoolPointer<LexerData> Lexer::run()
{
	ALIAS(data->values, values);
	ALIAS(data->tokens, tokens);

	/*size_t*/ 
	size_t i = 0;
	while (i < textLength)
	{
		processComments(i);
		processQuotes(i);

		if (safe_isgraph(text[i]))
			//non-space symbol
		{
			values->push_back(text[i++]);

			//other token starts
			if (Search::binary(core.breakChars, text[i]) != -1 || Search::binary(core.breakChars, text[i - 1]) != -1 || charIsTextChar(text[i - 1]) != charIsTextChar(text[i]))
			{
				hasSpacesPost = false;
				newToken();		//std::cout << "ntoken\t" << i << "\n";

			}
		}
		else
		{
			//space
			i++;

			hasSpacesPost = true;
			newToken();		//std::cout << "ntoken\t" << i << "\n";

		}
	}

	hasSpacesPost = true;
	newToken();

	//print_ldata(*data);

	return std::move(data);
}