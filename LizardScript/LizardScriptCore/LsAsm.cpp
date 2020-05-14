#include "pch.h"
#include <vector>
#include <memory>
#include <stdlib.h>
#include <iostream>

#include "LsAsm.h"
#include "OpcodesText.h"
#include "LsTypedefs.h"

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
	core.simpleKeywords.push_back(std::unique_ptr<KeywordToken>(new KeywordToken("(", KeywordTokenType::LeftBracket)));
	core.simpleKeywords.push_back(std::unique_ptr<KeywordToken>(new KeywordToken(")", KeywordTokenType::RightBracket)));

	core.simpleKeywords.push_back(std::unique_ptr<KeywordToken>(
		&((new KeywordToken("byte", KeywordTokenType::Simple))->setCustom((void*)1001))));
	core.simpleKeywords.push_back(std::unique_ptr<KeywordToken>(
		&((new KeywordToken("ofs", KeywordTokenType::Simple))->setCustom((void*)1002))));
	core.simpleKeywords.push_back(std::unique_ptr<KeywordToken>(
		&((new KeywordToken("int", KeywordTokenType::Simple))->setCustom((void*)1003))));
	core.simpleKeywords.push_back(std::unique_ptr<KeywordToken>(
		&((new KeywordToken("int64", KeywordTokenType::Simple))->setCustom((void*)1004))));

	auto* kw1 = new KeywordToken("label", KeywordTokenType::Simple);
	kw1->customData = (void*)2;
	core.simpleKeywords.push_back(std::unique_ptr<KeywordToken>(kw1));

	core.confirmChanges();

	return core;
}

enum class LsAsmType
{
	INT64,
	INT,
	OFFSET_T,
	BYTE,
	//STDSTRING,
};

template <typename T>
T parseInteger(TCHAR* t)
{
	TCHAR* end;

	T value = strtoll(t, &end, 10);
	if (end == t)
		throw Exception("Cannot parse token ", t);
	return value;
}

LsFunction LsAsm::compile(const TCHAR* text, size_t length)
{
	LsFunction f;
	std::map<TCHAR*, LsInternalAddr, cmp_str> labels;

	auto _lexerData = runLexer(core, text, length);
	auto& lexerData = *_lexerData;

	//lexerData.print();

	PoolPointer<std::stack<LsAsmType>> parsingTypes;
	parsingTypes->push(LsAsmType::BYTE);

	for (size_t i = 0; i < lexerData.tokens->size(); i++)
	{
		KeywordToken* kwtoken;

		if (lexerData.tryGetKeyword(i, kwtoken))
		{
			if (kwtoken->customData == (void*)1) //any command
			{
				if (textToOpcode.find(kwtoken->value) != textToOpcode.end())
				{
					f.code.push_back(textToOpcode[kwtoken->value]);
				}
				else throw Exception("Unknown instruction ", kwtoken->value);
			}
			else if (kwtoken->customData == (void*)2) //label
			{
				if (kwtoken->value[0] == 'l')
				{
					TCHAR* t; size_t tIndex;

					i++;

					if (lexerData.tryGetValue(i, t, &tIndex))
						labels.insert(std::make_pair(t, f.code.size()));
					else throw Exception("An odd label name at ", i, "/", lexerData.tokens->size());
				}
			}
			else if (kwtoken->customData >= (void*)1000)
			{
				if (kwtoken->customData == (void*)1001)
					parsingTypes->top() = LsAsmType::BYTE;
				else if (kwtoken->customData == (void*)1002)
					parsingTypes->top() = LsAsmType::OFFSET_T;
				else if (kwtoken->customData == (void*)1003)
					parsingTypes->top() = LsAsmType::INT;
				else if (kwtoken->customData == (void*)1004)
					parsingTypes->top() = LsAsmType::INT64;
			}
			else if (kwtoken->type == KeywordTokenType::LeftBracket)
			{
				parsingTypes->push(parsingTypes->top());
			}
			else if (kwtoken->type == KeywordTokenType::RightBracket)
			{
				parsingTypes->pop();
			}
			else if (kwtoken->compilerFlags == CompilerFlags::String)
			{
				i++;
				TCHAR* t; size_t tIndex;
				if (lexerData.tryGetValue(i, t, &tIndex))
				{
					f.push_str(t);
				}
				else throw Exception("Invalid string!");
			}
		}
		else
		{
			TCHAR* t; size_t tIndex;
			if (lexerData.tryGetValue(i, t, &tIndex))
			{
				//get label. label is always LsInternalAddr.
				if (labels.find(t) != labels.end())
				{
					LsInternalAddr s = labels[t];
					for (size_t i = 0; i < sizeof(s); i++)
						f.code.push_back(((char*)&s)[i]);
				}
				else
				{
					switch (parsingTypes->top())
					{
						case LsAsmType::INT64:
						{					
							f.push_back(parseInteger<int64>(t));
							break;
						}						
						case LsAsmType::INT:
						{
							f.code.push_back(parseInteger<int>(t));
							break;
						}
						case LsAsmType::OFFSET_T:
						{
							f.push_back(parseInteger<OFFSET_T>(t));
							break;
						}
						case LsAsmType::BYTE:
						{
							f.code.push_back(parseInteger<unsigned char>(t));
							break;
						}
					}


				}
			}
			else throw Exception("An odd token at ", i, "/", lexerData.tokens->size());
		}
	}

	//for (auto& l : labels)
	//	std::cout << l.first << l.second;

	return f;
}