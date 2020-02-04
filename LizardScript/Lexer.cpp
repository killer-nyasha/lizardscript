#include "stdafx.h"
#include "Lexer.h"
#include "algorithms.h"
#include "Tokens.h"

using namespace LizardScript;

KeywordToken Lexer::stringKw = KeywordToken("__string__");
OperatorToken Lexer::callKw = OperatorToken("__call__", Arity::Unary, 1);

void Lexer::init()
{
	textLength = _tcslen(text);
	size_t nCapacity = textLength + 256;
	if (data->values.capacity() < nCapacity)
		data->values.reserve(nCapacity);

	data->values.resize(0);
	data->tokens.resize(0);
}

Lexer::Lexer(SyntaxCore& c, const TCHAR* t)
	: core(c), text(t)
{
	init();
}

bool Lexer::addFromList(const std::vector<KeywordToken*>& list, KeywordToken& pseudoKw)
{
	ALIAS(data->values, values);
	ALIAS(data->tokens, tokens);

	int index = pBinarySearch(list, &pseudoKw);

	if (index != -1)
	{
		//add keyword
		KeywordToken* kw = list[index];
		tokens.push_back(kw->value);
		values.resize(lastValueIndex);

		lastKeywordType = kw->type;
		return true;
	}
	else
	{
		//add non-keyword
		tokens.push_back(&values[lastValueIndex]);
		lastValueIndex = values.size();

		lastKeywordType = KeywordTokenType::Unary;
		return false;
	}
}

void Lexer::newToken()
{
	ALIAS(data->values, values);
	ALIAS(data->tokens, tokens);

	if (values.size() != lastValueIndex)
	{
		values.push_back(0);

		KeywordToken pseudoKw = KeywordToken(&(values)[lastValueIndex]);
		int aIndex = pBinarySearch(core.keywords_listA, &pseudoKw);

		if (kwtype_before_listA(lastKeywordType))
			addFromList(core.keywords_listA, pseudoKw);
		else if (kwtype_before_listB(lastKeywordType))
			addFromList(core.keywords_listB, pseudoKw);
		else throw Exception("Is's a fucking odd keyword");

	}
}

PoolPointer<LexerData> Lexer::run()
{
	ALIAS(data->values, values);
	ALIAS(data->tokens, tokens);

	size_t i = 0;
	while (i < textLength)
	{

		////comments
		//if (i + 1 < textLength && text[i] == '/' && text[i + 1] == '*')
		//{
		//	int level = 1;
		//	i++;
		//	while (level != 0 && i + 1 < textLength)
		//	{
		//		if (text[i] == '/' && text[i + 1] == '*')
		//			level++;
		//		if (text[i] == '*' && text[i + 1] == '/')
		//			level--;
		//		i++;
		//	}
		//	i++;
		//}
		//else if (i + 1 < textLength && text[i] == '/' && text[i + 1] == '/')
		//	while (text[i] != '\r' && text[i] != '\n')
		//		i++;


		////string literals
		//else if (text[i] == '[')
		//{
		//	int level = 1;
		//	i++; newToken();

		//	values.push_back('$');
		//	newToken();

		//	while (level > 0 && i < textLength)
		//	{
		//		if (text[i] == '[')
		//			level++;
		//		if (text[i] == ']')
		//		{
		//			level--;
		//			if (level == 0) break;
		//		}
		//		values.push_back(text[i++]);
		//	}
		//	if (text[i] != ']')
		//	{
		//		throw Exception("Unclosed quote.");
		//	}
		//	i++; newToken();
		//}
		//else if (text[i] == '\"')
		//{
		//	i++; newToken();

		//	values.push_back('$');
		//	newToken();

		//	while (text[i] != '\"' && i < textLength)
		//		values.push_back(text[i++]);
		//	if (text[i] != '\"')
		//	{
		//		throw Exception("Unclosed quote.");
		//	}
		//	i++; newToken();
		//}
		//else if (text[i] == '\`')
		//{
		//	i++; newToken();

		//	while (text[i] != '\`' && i < textLength)
		//		values.push_back(text[i++]);
		//	if (text[i] != '\`')
		//	{
		//		throw Exception("Unclosed quote.");
		//	}
		//	i++; newToken();
		//}

		//other
		//else
		{
			if (safe_isgraph(text[i]))
			{
				values.push_back(text[i++]);

				if (binarySearch(core.breakChars, text[i]) != -1
					|| binarySearch(core.breakChars, text[i - 1]) != -1
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

	return std::move(data);
}