#include "pch.h"
#include <vector>
#include <memory>
#include <cstdlib>
#include <iostream>

#include "LsAsm.h"
#include "OpcodesText.h"

using namespace LizardScript;

SyntaxCore LsAsm::createSyntaxCore()
{
	SyntaxCore core;
	core.textChars = { '_' };
	core.breakChars = { '(', ')', '.', ';' };

	fillOpcodeToText();
	for (auto& elem : textToOpcode)
	{
		auto* kw = new KeywordToken(elem.first, KeywordTokenType::Simple);
		kw->customData = (void*)1;
		core.simpleKeywords.push_back(std::unique_ptr<KeywordToken>(kw));
	}

	core.simpleKeywords.push_back(std::unique_ptr<KeywordToken>(new KeywordToken(",", KeywordTokenType::Binary)));
	core.simpleKeywords.push_back(std::unique_ptr<KeywordToken>(new KeywordToken("[", KeywordTokenType::PrefixUnary)));
	core.simpleKeywords.push_back(std::unique_ptr<KeywordToken>(new KeywordToken("]", KeywordTokenType::PostfixUnary)));

	//set_vector<std::unique_ptr<KeywordToken>, KeywordToken*>(core.simpleKeywords,
	//	{
	//		new KeywordToken("(", KeywordTokenType::LeftBracket),
	//		new KeywordToken(")", KeywordTokenType::RightBracket),
	//	});

	//set_vector<std::unique_ptr<OperatorToken>, OperatorToken*>(core.prefixUnary,
	//	{
	//		new OperatorToken("-", KeywordTokenType::PrefixUnary, 70),
	//	});

	//set_vector<std::unique_ptr<OperatorToken>, OperatorToken*>(core.postfixUnary,
	//	{
	//		new OperatorToken("-", KeywordTokenType::PostfixUnary, 60),
	//		new OperatorToken("--", KeywordTokenType::PostfixUnary, 60),
	//	});

	//set_vector<std::unique_ptr<OperatorToken>, OperatorToken*>(core.binaryOperators,
	//	{
	//		new OperatorToken("=", KeywordTokenType::Binary, 20, Associativity::Right),

	//		new OperatorToken(">", KeywordTokenType::Binary, 30),
	//		new OperatorToken("<", KeywordTokenType::Binary, 30),

	//		new OperatorToken("+", KeywordTokenType::Binary, 50),
	//		new OperatorToken("-", KeywordTokenType::Binary, 50),
	//		new OperatorToken("*", KeywordTokenType::Binary, 50),
	//		new OperatorToken("/", KeywordTokenType::Binary, 50),

	//		new OperatorToken("--", KeywordTokenType::Binary, 50),
	//	});

	core.confirmChanges();

	return core;
}

LsFunction LsAsm::assemble(const TCHAR* text, size_t length)
{
	LsFunction f;

	auto _lexerData = runLexer(core, text, length);
	auto& lexerData = *_lexerData;

	for (size_t i = 0; i < lexerData.tokens->size(); i++)
	{
		KeywordToken* kwtoken;

		if (lexerData.tryGetKeyword(i, kwtoken))
		{
			if (kwtoken->customData != 0)
			{
				if (textToOpcode.find(kwtoken->value) != textToOpcode.end())
				{
					f.code.push_back(textToOpcode[kwtoken->value]);
				}
				//std::cout << kwtoken->value << std::endl;
			}
		}
		else
		{
			TCHAR* t; size_t tIndex;
			if (lexerData.tryGetValue(i, t, &tIndex))
			{
				int value = std::atoi(t);
				f.code.push_back(value);
			}
		}
	}

	return f;
}