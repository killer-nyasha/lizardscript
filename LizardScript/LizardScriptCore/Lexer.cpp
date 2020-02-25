#include "pch.h"
#include "Lexer.h"
#include "algorithms.h"
#include "Tokens.h"
#include "interpreter_util.h"
#include "SyntaxCore.h"
#include "Exception.h"

using namespace LizardScript;

KeywordToken Lexer::stringKw = KeywordToken("__string__");
OperatorToken Lexer::callKw = OperatorToken("__call__", Arity::Unary, 1);

void Lexer::init()
{
	data->values.resize(0);
	data->tokens.resize(0);
}

Lexer::Lexer(const SyntaxCore& c, const TCHAR* t)
	: core(c), text(t)
{
	init();
}

bool Lexer::addFromList(const std::vector<KeywordToken*>& list, KeywordToken& pseudoKw)
{
	ALIAS(data->values, values);
	ALIAS(data->tokens, tokens);

	int index = pSearch::binary<KeywordToken*>(list, &pseudoKw);

	if (index != -1)
	{
		//add keyword
		KeywordToken* kw = list[index];
		tokens.push_back(reinterpret_cast<void*>(kw->value));
		values.resize(lastValueIndex);
		lastKeywordType = kw->type;
		return true;
	}
	else
	{
		//add non-keyword

#ifdef _DEBUG
		if (lastValueIndex >= data->minReserved() && lastValueIndex < data->maxReserved())
		{ }
		else throw Exception("too many tokens");
#endif	

		tokens.push_back(reinterpret_cast<void*>(lastValueIndex));
		lastValueIndex = values.size();

		lastKeywordType = KeywordTokenType::Simple;

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

	size_t textLength = _tcslen(text);
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

				if (Search::binary(core.breakChars, text[i]) != -1
					|| Search::binary(core.breakChars, text[i - 1]) != -1
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