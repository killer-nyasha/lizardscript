#include "pch.h"
#include <vector>
#include <memory>
#include <stdlib.h>
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

	auto* kw1 = new KeywordToken("label", KeywordTokenType::Simple);
	kw1->customData = (void*)2;
	core.simpleKeywords.push_back(std::unique_ptr<KeywordToken>(kw1));

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

	std::map<TCHAR*, size_t, cmp_str> labels;

	auto _lexerData = runLexer(core, text, length);
	auto& lexerData = *_lexerData;

	for (size_t i = 0; i < lexerData.tokens->size(); i++)
	{
		KeywordToken* kwtoken;

		if (lexerData.tryGetKeyword(i, kwtoken))
		{
			if (kwtoken->customData == (void*)1)
			{
				if (textToOpcode.find(kwtoken->value) != textToOpcode.end())
				{
					f.code.push_back(textToOpcode[kwtoken->value]);
				}
				else throw Exception("unknown instruction ", kwtoken->value);

				//std::cout << kwtoken->value << std::endl;
			}
			else if (kwtoken->customData == (void*)2)
			{
				if (kwtoken->value[0] == 'l')
				{
					TCHAR* t; size_t tIndex;

					i++;

					if (lexerData.tryGetValue(i, t, &tIndex))
						labels.insert(std::make_pair(t, f.code.size()));
					else throw Exception();
				}
			}
		}
		else
		{
			TCHAR* t; size_t tIndex;
			if (lexerData.tryGetValue(i, t, &tIndex))
			{
				if (labels.find(t) != labels.end())
				{
					int s = labels[t];
					for (size_t i = 0; i < sizeof(s); i++)
						f.code.push_back(((char*)&s)[i]);
				}
				else
				{
					TCHAR* end;
					int value = strtoll(t, &end, 10);

					if (end == t)
						throw Exception();

					f.code.push_back(value);
				}
			}
			else throw Exception();
		}
	}

	return f;
}