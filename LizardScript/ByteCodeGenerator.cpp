#include "stdafx.h"

#include "ByteCodeGenerator.h"
#include "LizardScriptLibrary.h"

using namespace LizardScript;

#include "Jumps.h"
#include "IdentifiersProcessor.h"
#include "FindType.h"

#include "stringptr.h"

#include "LsOpcode.h"

template <typename T>
struct vector_rewrite
{
	std::vector<T>& source;
	size_t size;

	vector_rewrite(std::vector<T>& source) : source(source)
	{
		size = 0;
	}

	void push_back(T& value)
	{
		source[size++] = value;
	}

	void push_back(T&& value)
	{
		source[size++] = value;
	}
};

void ByteCodeGenerator::parseLiteral(TCHAR* token)
{
	TCHAR* z1 = nullptr, * z2 = nullptr;

	int radix = 10;

	int len = _tcslen(token);

	if (token[0] == '0' && token[1] == 'x')
	{
		radix = 16;
		token += 2;
	}
	else if (token[0] == '0' && token[1] == 'b')
	{
		radix = 2;
		radix = 16;
	}

	int intValue = (int)_tcstoll(token, &z1, 10);
	//float fval = (float)_tcstof(token, &z2);

	if (z1 != token)
	{
		//int token

		if (optStack->check(intValue))
		{
			optStack->push(intValue);
		}
		else
		{
			//инкапсулировать в метод, в который запихиваешь любой тип???
			//typed_reg& rn = reg.alloc(TYPEINFO(int));
			//code << LsOpcode::push_32 << rn;

			try_push_static(intValue);
		}

		//typed_reg& rn = reg.alloc(makeTypeInfo<float>());
		//code << LsOpcode::push_32;
		//code << rn;

		//if (z2 > z1)
		//{
		//	code << fval;
		//	rn.type = makeTypeInfo<float>();
		//}
		//else
		//{
		//	code << ival;
		//	rn.type = makeTypeInfo<int>();
		//}
	}
	else
	{
		//identifier
		//identifiersProcessor(ptoken);
	}
}

ByteCodeGenerator::ByteCodeGenerator(SemanticCore& core, std::vector<TCHAR*>& tokens, TypeInfo type, bool optimized)
	: core(core), code(e->code), tokens(tokens), type(type), localVarMaxOffset(e->maxStackSize), optimized(optimized)
{
	e->type = type;

	for (auto ptoken = tokens.begin(); ptoken < tokens.end(); ptoken++)
	{
		//index of the current token
		int tIndex = ptoken - tokens.begin();
		//text value of the current token
		auto token = *ptoken;

		if (KeywordToken::isKeyword(token))
		{
			auto kwtoken = reinterpret_cast<KeywordToken*>(token);
			core.findOperator(kwtoken);
		}
		else
		{

		}
	}



































	//short int stackofs = 0;

	//TCHAR* className = nullptr;
	//int classDecl = -111;

	//localVarLevels->push(0);
	//functionCallsLevels->push(0);

	//int startLineIndex = 0;

	/*
	for (auto ptoken = tokens.begin(); ptoken < tokens.end(); ptoken++)
	{
		//index of current token
		int tIndex = ptoken - tokens.begin();

		auto token = *ptoken;

		processJumps(tIndex);

		//comments
		if (!optimized)
		{
			code << LsOpcode::comment;
			code << regindex(0);
			int len = _tcslen(*ptoken);
			code << len;
			for (size_t i = 0; i < len; i++)
				code << (*ptoken)[i];
		}

		if (KeywordToken::isKeyword(token))
		{
			auto kwtoken = reinterpret_cast<KeywordToken*>(token);

			if (kwtoken->checkFlag(KeywordFlags::EndLine) || 
				kwtoken->checkSpecial(SpecialKeywords::LeftBrace) || 
				kwtoken->checkSpecial(SpecialKeywords::RightBrace) ||
				kwtoken->checkSpecial(SpecialKeywords::Else))
			{
				startLineIndex = code.data.size();// tIndex;
				reg.clear();

				if (kwtoken->checkSpecial(SpecialKeywords::LeftBrace))
				{
					localVarLevels.push(localVar.size());
				}
				else if (kwtoken->checkSpecial(SpecialKeywords::RightBrace))
				{
					////set local var offsets
					//for (auto& a : localVarAddr)
					//{
					//	for (size_t i = localVar.size() - 1; i + 1 >= 1; i--)
					//	{
					//		if (localVar[i].offset == 0)
					//			break;

					//		if (_tcscmp(&localVar[i].name[0], a.varName) == 0)
					//		{
					//			*(short*)&(code.data[a.byteCodeOffset]) = localVar[i].offset;
					//			break;
					//		}
					//	}
					//	//вынести в отдельную функцию всё это?
					//	//throw Exception(std::string("local variable ") + a.varName + " was lost :(");
					//}

					if (classDecl == localVarLevels.size()-1)
					{
						size_t lastLvl = localVarLevels.top();

						TypeInfo info = makeTypeInfo<void>();
						info.lsOwnClassName = new std::string(className);

						info.byValueSize = localVarOffset - localVar[lastLvl].offset;
						info.ptr = 0;

						TypeInfoEx ex = TypeInfoEx(info);
						//ex.printFunction = &print<std::ostream>;

						for (size_t i = lastLvl; i < localVar.size(); i++)
						{
							FieldInfo field = localVar[i];
							field.offset -= localVar[lastLvl].offset;
							ex.members.push_back(field);
						}

						globalMetadataTable.insert(std::make_pair(info, ex));

						className = nullptr;
						classDecl = -111;

						print(std::cout, (char*)this, info);
					}

					//pop local variables
					size_t localVarNewSize = localVarLevels.top();
					localVarLevels.pop();
					localVar.resize(localVarNewSize);
					if (localVar.size() > 0)
						localVarOffset = localVar[localVar.size() - 1].offset + localVar[localVar.size() - 1].type.size();
					else localVarOffset = 0;
				}
			}
			else if (kwtoken->checkSpecial(SpecialKeywords::If))
			{
				PossibleFunctionCalls call;
				call.isIf = true;

				call.codeIndex = (int)code.data.size();
				int ntIndex = findEndLine(ptoken);
				call.ntokenIndex = ntIndex;

				if (ntIndex + 1 < tokens.size() && reinterpret_cast<KeywordToken*>(tokens[ntIndex + 1])->checkSpecial(SpecialKeywords::Else))
				{
					call.elseNtokenIndex = findEndLine(tokens.begin() + ntIndex + 1);
				}

				functionCalls.push(call);
			}
			else if (kwtoken->checkSpecial(SpecialKeywords::While))
			{
				PossibleFunctionCalls call;
				call.isWhile = true;

				call.codeIndex = (int)code.data.size();
				int ntIndex = findEndLine(ptoken);
				call.ntokenIndex = ntIndex;
				call.tokenIndex = startLineIndex;

				functionCalls.push(call);
			}
			else if (kwtoken->checkSpecial(SpecialKeywords::Class))
			{
				className = *(ptoken++);
				classDecl = (int)localVarLevels.size();
			}
			else if (kwtoken->checkSpecial(SpecialKeywords::LeftBracket))
			{
				functionCallsLevels.push(functionCalls.size());
			}
			else if (kwtoken->checkSpecial(SpecialKeywords::RightBracket))
			{
				functionCallsLevels.pop();

				while (functionCalls.size() > functionCallsLevels.top())
				{
					PossibleFunctionCalls call = functionCalls.top();
					functionCalls.pop();

					if (call.isIf)
					{
						auto r = reg.free();
						open_reg(r, 0);

						code << opcode::jz << r;
						jmps.push_back(jmp{ (int)code.data.size(), call.ntokenIndex, -1 });
						code << (int)0;

						if (call.elseNtokenIndex != 0)
						{
							fjmps.push_back(futurejmp{ call.ntokenIndex, call.elseNtokenIndex });
						}
					}
					else if (call.isWhile)
					{
						auto r = reg.free();
						open_reg(r, 0);

						code << opcode::jz << r;
						jmps.push_back(jmp{ (int)code.data.size(), call.ntokenIndex, call.tokenIndex });
						code << (int)0;
					}
					else
					{
						findFunctionToCall(call);
					}
				}

			}
			else if (kwtoken->checkSpecial(SpecialKeywords::New) || kwtoken->checkSpecial(SpecialKeywords::NewLocal))
			{
				TypeInfo t = findType(*(ptoken+1));
				
				if (kwtoken->checkSpecial(SpecialKeywords::New))
				{
					typed_reg& r = reg.alloc(t);
					code << opcode::alloc << r;
					code << (short)t.size();
					r.type.ptr++;
				}
				else
				{
					FieldInfo& f = newLocalVariable(t, "_temp_");
				}

				bool isOk = false;
				PossibleFunctionCalls calls;
				calls.index = reg.stackEmul.top().index;
				for (auto& metadata : make_reverse_wrapper(globalMetadataTable[t].members.get<FunctionInfo>()))
				{
					if (_tcscmp(&metadata.name[0], "ctor") == 0)
					{
						calls.functions.push_back(&metadata);
						isOk = true;
					}
				}

				if (isOk)
					functionCalls.push(calls);
				else WARNING(std::string("Any constructors of ") + t.text() + " wasn't found.");

				ptoken++;
			}
			else if (kwtoken->checkSpecial(SpecialKeywords::This))
			{
				identifiersProcessor(ptoken);
			}
			else if (kwtoken->checkSpecial(SpecialKeywords::Null))
			{
				typed_reg& rn = reg.alloc(makeTypeInfo<nullptr_t>());
				code << (is_x64() ? opcode::push_64 : opcode::push_32) << rn << (void*)nullptr;
			}
			else
			{
				typed_reg r1 = reg.free();
				if (!addUnary(kwtoken, r1))
				{
					typed_reg r2 = reg.free();
					addBinary(kwtoken, r2, r1);
				}
			}

		}
		else if (token[0] == '$')
		{
			typed_reg& rn = reg.alloc(makeTypeInfo<stringptr>());
			code << opcode::push_stringptr;
			code << rn;

			ptoken++;
			int len = _tcslen(*ptoken);
			code << len;
			for (size_t i = 0; i < len; i++)
				code << (*ptoken)[i]; 
		}
		else 
		{
			TCHAR* z1 = nullptr, *z2 = nullptr;
			int ival = (int)_tcstoll(token, &z1, 10);
			float fval = (float)_tcstof(token, &z2);

			if (z1 != token)
			{
				typed_reg& rn = reg.alloc(makeTypeInfo<float>());
				code << opcode::push_32;
				code << rn;

				if (z2 > z1)
				{
					code << fval;
					rn.type = makeTypeInfo<float>();
				}
				else
				{
					code << ival;
					rn.type = makeTypeInfo<int>();
				}
			}
			else
			{
				identifiersProcessor(ptoken);
			}
		}
	}
	*/

	code << LsOpcode::ret << regindex_pair(0);
}
