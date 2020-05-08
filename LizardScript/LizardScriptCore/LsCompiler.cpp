#include "pch.h"
#include "LsCompiler.h"
#include "i_lexer.h"
#include "i_parser.h"

#include "Opcodes.hxx"
#include "OpcodesText.h"
#include "NonTypedStack2.h"

using namespace LizardScript;

LsFunction LsCompiler::compile(const TCHAR* text, size_t length)
{
	LsFunction f;

	auto _lexerData = runLexer(core, text, length);
	auto& lexerData = *_lexerData;
	
	lexerData.print();
	runParser(lexerData);
	lexerData.print();

	NonTypedStack2 stack;

	for (size_t i = 0; i < lexerData.tokens->size(); i++)
	{
		KeywordToken* kwtoken;

		if (lexerData.tryGetKeyword(i, kwtoken))
		{
			if (kwtoken->is_operator())
			{
				OperatorToken* operatorToken = OperatorToken::asOperator(kwtoken);

				if (operatorToken->type == KeywordTokenType::Binary)
				{
					//if (operatorToken->value == )

					//textToOpcode[];

				}
				else if (operatorToken->type == KeywordTokenType::PrefixUnary ||
					operatorToken->type == KeywordTokenType::PostfixUnary)
				{

				}
			}
		}
		else
		{
			TCHAR* t; size_t tIndex;
			if (lexerData.tryGetValue(i, t, &tIndex))
			{
				//парсим число
				TCHAR* end;
				int64 value = strtoll(t, &end, 10);

				if (end != t)
				{
					stack.push(value);
				}
				else
				{
					//не удалось распарсить
				}

			}
			else throw Exception();
		}
	}
}