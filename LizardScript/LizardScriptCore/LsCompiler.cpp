#include "pch.h"
#include "LsCompiler.h"
#include "i_lexer.h"
#include "i_parser.h"

#include "Opcodes.hxx"
#include "OpcodesText.h"
#include "NonTypedStack2.h"
#include "Runtime.h"

using namespace LizardScript;

char operator_name[512];

class OperatorsForType
{
	static std::map<const char*, const char*, cmp_str> st_operators;

public:

	std::map<const char*, LsCode, cmp_str> operators;

	//!requires opcodes_initialized
	OperatorsForType(const char* type1, const char* type2 = nullptr)
	{
		size_t len1 = strlen(type1);
		size_t len2 = type2 ? strlen(type2) : 0;

		size_t t_count = (type2 ? 2 : 1);

		for (auto& pair : st_operators)
		{
			const char* mnem = pair.second;
			size_t len0 = strlen(mnem);

			//char* operator_name = new char[len0 + len1 + len2 + t_count + 1];

			memcpy(operator_name, mnem, len0);
			operator_name[len0] = '_';
			memcpy(operator_name + len0 + 1, type1, len1);
			operator_name[len0 + len1 + 1] = '_';

			if (type2)
				memcpy(operator_name + len0 + len1 + 2, type2, len2);

			operator_name[len0 + len1 + len2 + t_count] = '\0';

			//std::cout << operator_name << std::endl;

			if (textToOpcode[operator_name] != 0)
			operators.insert(std::make_pair(pair.first, (LsCode)textToOpcode[operator_name]));
		
			//delete[] operator_name;
		}
	}

	//~OperatorsForType()
	//{
	//	for (auto& pair : operators)
	//	{
	//		delete[] pair.first;
	//	}
	//}
};

std::map<const char*, const char*, cmp_str> OperatorsForType::st_operators =
{
	{ "+", "add" },
	{ "-", "sub" },
	{ "*", "mul" },
	{ "/", "div" },
	{ "==", "eq" },
	{ "!=", "not_eq" },
	{ ">", "more" },
	{ "<", "less" },
	{ ">=", "more_eq" },
	{ "<=", "less_eq" },
	{ "=", "mov" },
};


LsFunction LsCompiler::compile(const TCHAR* text, size_t length)
{
	fillOpcodeToText();

	OperatorsForType operators_int64("int64");

	LsFunction f;

	Runtime runtime;

	LsFunction fTemp;

	auto _lexerData = runLexer(core, text, length);
	auto& lexerData = *_lexerData;
	
	lexerData.print();
	runParser(lexerData);
	lexerData.print();

	NonTypedStack2 tempStack;

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
					if (operators_int64.operators.find(operatorToken->value) != operators_int64.operators.end())
					{
						LsCode opcode = operators_int64.operators[operatorToken->value];
						
						TempValue val1 = tempStack.pop();
						TempValue val2 = tempStack.pop();

						//compile-teme evaluate
						if (val1.compileTime && val2.compileTime)
						{
							//HARDCODED FOR INT64!!!
							runtime.setLocal<int64>(0, val1.d.value);
							runtime.setLocal<int64>(8, val2.d.value);

							fTemp.code.push_back(opcode);
							fTemp.push_back((OFFSET_T)0);
							fTemp.push_back((OFFSET_T)8);
							fTemp.code.push_back(LsAsm::ret);


							runtime.run(fTemp);
							fTemp.code.clear();

							TempValue retVal = val1;
							retVal.d.value = runtime.getLocal<int64>(0);

							tempStack.push(retVal.d.value);
						}

						//f.code.push_back(opcode);
					}
					else throw Exception("Cannot find operator ", operatorToken->value, " for ", "int64");

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
					tempStack.push(value);
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