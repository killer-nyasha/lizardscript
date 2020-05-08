#include "pch.h"
#include "LsCompiler.h"
#include "i_lexer.h"
#include "i_parser.h"

using namespace LizardScript;

LsFunction LsCompiler::compile(const TCHAR* text, size_t length = 0)
{
	LsFunction f;

	auto _lexerData = runLexer(core, text, length);
	auto& lexerData = *_lexerData;
	runParser(lexerData);

	for (size_t i = 0; i < lexerData.tokens->size(); i++)
	{
		KeywordToken* kwtoken;

		if (lexerData.tryGetKeyword(i, kwtoken))
		{

		}
		else
		{
			TCHAR* t; size_t tIndex;
			if (lexerData.tryGetValue(i, t, &tIndex))
			{

			}
			else throw Exception();
		}
	}
}