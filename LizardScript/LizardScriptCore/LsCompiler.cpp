#include "pch.h"
#include "LsCompiler.h"
#include "i_lexer.h"
#include "i_parser.h"

#include "Opcodes.hxx"
#include "NonTypedStack2.h"
#include "Runtime.h"

#include "Lsl.h"

#include "metagen_system.h"

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
		else throw Exception("Can't find an opcode for ", operator_name);
	}

	return std::move(retMap);
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
	{ ".", "add" },
};

OperatorsMap s_map_tchar_prefix =
{
	//{ "print", "out" },
};

struct FunctionCall
{
	const TCHAR* name;
};

void LsCompiler::addBinaryMap(OperatorsMap& m, const TCHAR* text1, TypeInfo info1)
{
	auto newMap = Operators::forType(m, text1);
	map_binary.insert(std::make_pair(info1, std::move(newMap)));
}

void LsCompiler::addPrefixMap(OperatorsMap& m, const TCHAR* text1, TypeInfo info1)
{
	auto newMap = Operators::forType(m, text1);
	map_prefix.insert(std::make_pair(info1, std::move(newMap)));
}

void LsCompiler::addPostfixMap(OperatorsMap& m, const TCHAR* text1, TypeInfo info1)
{
	auto newMap = Operators::forType(m, text1);
	map_postfix.insert(std::make_pair(info1, std::move(newMap)));
}


LsCompiler::LsCompiler(SyntaxCore& core) : core(core)
{
	fillOpcodeToText();

	addBinaryMap(s_map_int64_binary, "int64", typeInfo<int64>());
	addBinaryMap(s_map_tchar_binary, "stdstring", typeInfo<std::string>());

	addPrefixMap(s_map_tchar_prefix, "stdstring", typeInfo<std::string>());
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
	size_t currentBracketAction = SIZE_MAX;

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
						//function name
						auto& fName = tempStack[currentBracketAction - 1];

						std::string& fn = fName.d.get<std::string>();

						if (globalMetadataTable.find(typeInfo<Lsl>()) == globalMetadataTable.end())
							throw Exception("Cannot find Lsl metadata");

						auto& lslFunctions = globalMetadataTable[typeInfo<Lsl>()].members.get<FunctionInfo>();

						for (auto& f : lslFunctions)
						{
							if (f.name != fn)
								continue;

							//push arguments
							for (size_t i = currentBracketAction; i < tempStack.size(); i++)
							{

							}
						}

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

								runtime.setLocal(0, val1.d);
								runtime.setLocal(64, val2.d);

								fTemp.code.push_back(opcode);
								fTemp.push_back((OFFSET_T)0);
								fTemp.push_back((OFFSET_T)64);
								fTemp.code.push_back(LsAsm::ret);

								runtime.run(fTemp);
								fTemp.code.clear();

								TempValue retVal = val1;
								retVal.d = runtime.getLocalSt<int64>(0);

								tempStack.push(retVal.d);
							}
							else throw Exception("Runtime evaluation isn't implemented");

							//f.code.push_back(opcode);
						}
						else throw Exception("Cannot find operator ", operatorToken->value, " for ", val1.d.type.to_string());

					}
					else
					{
						throw Exception("Cast operators aren't implemented");
					}

				}
				else if (operatorToken->type == KeywordTokenType::PrefixUnary ||
					operatorToken->type == KeywordTokenType::PostfixUnary)
				{
					//do something
					throw Exception("Unary operators aren't implemented");
				}
			}
			else if (kwtoken->type == KeywordTokenType::LeftBracket)
			{
				bracketsActions.push(tempStack.size());
			}
			else if (kwtoken->type == KeywordTokenType::RightBracket)
			{
				currentBracketAction = bracketsActions.top();
				bracketsActions.pop();
				//auto br = bracketsActions.top();
				//if (br)
				//{
					//CALL FUNCTION
				//int k = 1;
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
					//не удалось распарсить - пока считаем строкой?
					tempStack.push(std::string(t));
				}

			}
			else throw Exception();
		}
	}

	return f;
}