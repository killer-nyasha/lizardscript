#include "pch.h"
#include "LsCompiler.h"
#include "i_lexer.h"
#include "i_parser.h"

#include "Opcodes.hxx"
#include "NonTypedStack2.h"
#include "Runtime.h"

using namespace LizardScript;

char operator_name[512];

//!\warning requires opcodes_initialized
OpcodesMap Operators::forType(OperatorsMap& st_operators, const char* type1, const char* type2)
{
	OpcodesMap retMap;

	size_t len1 = strlen(type1);
	size_t len2 = type2 ? strlen(type2) : 0;

	size_t t_count = (type2 ? 2 : 1);

	for (auto& pair : st_operators)
	{
		const char* mnem = pair.second;
		size_t len0 = strlen(mnem);

		if (mnem[len0-1] == '\n')
		{
			memcpy(operator_name, mnem, len0);
			operator_name[len0 - 1] = '\0';
		}
		else
		{
			//char* operator_name = new char[len0 + len1 + len2 + t_count + 1];
			memcpy(operator_name, mnem, len0);
			operator_name[len0] = '_';
			memcpy(operator_name + len0 + 1, type1, len1);
			operator_name[len0 + len1 + 1] = '_';

			if (type2)
				memcpy(operator_name + len0 + len1 + 2, type2, len2);

			operator_name[len0 + len1 + len2 + t_count] = '\0';
		}

		if (textToOpcode[operator_name] != 0)
			retMap.insert(std::make_pair(pair.first, (LsCode)textToOpcode[operator_name]));
	}
}

OperatorsMap s_map_int64_binary =
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

OperatorsMap s_map_tchar_binary =
{
	{ ".", "test_operator\n" },
};


struct FunctionCall
{
	const TCHAR* name;
};

void LsCompiler::addBinaryMap(OperatorsMap& m, const TCHAR* text1, TypeInfo info1)
{
	auto newMap = Operators::forType(m, text1);
	map_binary.insert(std::make_pair(info1, newMap));
}

LsCompiler::LsCompiler(SyntaxCore& core) : core(core)
{
	fillOpcodeToText();

	addBinaryMap(s_map_int64_binary, "int64", typeInfo<int64>());
	addBinaryMap(s_map_tchar_binary, "string", typeInfo<std::string>());
}

LsFunction LsCompiler::compile(const TCHAR* text, size_t length)
{
	LsFunction f;

	Runtime runtime;

	LsFunction fTemp;

	auto _lexerData = runLexer(core, text, length);
	auto& lexerData = *_lexerData;
	
	lexerData.print();
	runParser(lexerData);
	lexerData.print();

	NonTypedStack2 tempStack;

	std::stack<size_t> bracketsActions;

	for (size_t i = 0; i < lexerData.tokens->size(); i++)
	{
		KeywordToken* kwtoken;

		if (lexerData.tryGetKeyword(i, kwtoken))
		{
			if (kwtoken->is_operator())
			{
				OperatorToken* operatorToken = OperatorToken::asOperator(kwtoken);

				if (operatorToken->compilerFlags != CompilerFlags::None)
				{ 
					if (operatorToken->compilerFlags == CompilerFlags::Call)
					{
						//bracketsActions.pop();
						//bracketsActions.push(true);
					}
					else throw Exception("Unknown compiler flags of operator ", operatorToken->value);
				}
				else if (operatorToken->type == KeywordTokenType::Binary)
				{
					TempValue val1 = tempStack.pop();
					TempValue val2 = tempStack.pop();

					if (val1.d.type == val2.d.type)
					{
						auto& operators = map_binary[val1.d.type];

						if (operators.find(operatorToken->value) != operators.end())
						{
							LsCode opcode = operators[operatorToken->value];

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
					else
					{
						throw Exception("cast operators aren't implemented");
					}

				}
				else if (operatorToken->type == KeywordTokenType::PrefixUnary ||
					operatorToken->type == KeywordTokenType::PostfixUnary)
				{

				}
			}
			else if (kwtoken->type == KeywordTokenType::LeftBracket)
			{
				bracketsActions.push(tempStack.size());
			}
			else if (kwtoken->type == KeywordTokenType::RightBracket)
			{
				//auto br = bracketsActions.top();
				//if (br)
				//{
					//CALL FUNCTION
				int k = 1;
				//}
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
					tempStack.push(new std::string(t));
				}

			}
			else throw Exception();
		}
	}
}