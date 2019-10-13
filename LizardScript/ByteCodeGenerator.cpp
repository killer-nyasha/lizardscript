#include "ByteCodeGenerator.h"
#include "LizardScriptLibrary.h"

using namespace LizardScript;

#include "Jumps.h"
#include "IdentifiersProcessor.h"
#include "FindType.h"

#include "stringptr.h"

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

std::stack<typed_reg> global_stackEmul;
std::stack<regindex> global_otherReg;
std::vector<jmp> global_jmps;
std::vector<futurejmp> global_fjmps;
std::vector<FieldInfo> global_localVar;
std::stack<PossibleFunctionCalls> global_functionCalls;

std::stack<size_t> global_localVarLevels;
std::stack<size_t> global_functionCallsLevels;

template <typename T>
void clear(std::stack<T>& st)
{
	while (st.size() > 0)
		st.pop();
}

ByteCodeGenerator::ByteCodeGenerator(std::vector<TCHAR*>& tokens, TypeInfo type, SyntaxCore& core)
	: jmps(global_jmps), fjmps(global_fjmps), localVar(global_localVar), functionCalls(global_functionCalls),
	code(e.code), core(core), tokens(tokens), type(type),
	reg(global_stackEmul, global_otherReg), localVarMaxOffset(e.maxStackSize)
{
	e.type = type;

	auto& localVarLevels = global_localVarLevels;
	clear(localVarLevels);
	localVarLevels.push(0);

	auto& functionCallsLevels = global_functionCallsLevels;
	clear(functionCallsLevels);
	functionCallsLevels.push(0);

	jmps.clear();
	fjmps.clear();
	localVar.clear();
	clear(functionCalls);

	Keyword2::core = &core;
	auto& keywords = initKeywords();

	//int& localVarMaxOffset = e.maxStackSize;
	//localVarMaxOffset = 0;

	int startLineIndex = 0;

	for (auto ptoken = tokens.begin(); ptoken < tokens.end(); ptoken++)
	{
		//index of current token
		int tIndex = ptoken - tokens.begin();

		auto token = *ptoken;

		processJumps(tIndex);

		if (core.isKeyword(token))
		{
			auto kwtoken = reinterpret_cast<Keyword*>(token);

			if (kwtoken->checkFlag(KeywordFlags::EndLine) || kwtoken->checkFlag(KeywordFlags::LeftBrace) || kwtoken->checkFlag(KeywordFlags::RightBrace) || kwtoken->checkFlag(KeywordFlags::Else))
			{
				startLineIndex = code.data.size();// tIndex;
				reg.clear();

				if (kwtoken->checkFlag(KeywordFlags::LeftBrace) > 1)
				{
					localVarLevels.push(localVar.size());
				}
				else if (kwtoken->checkFlag(KeywordFlags::RightBrace) > 1)
				{
					size_t localVarNewSize = localVarLevels.top();
					localVarLevels.pop();
					localVar.resize(localVarNewSize);

					if (localVar.size() > 0)
						localVarOffset = localVar[localVar.size() - 1].offset + localVar[localVar.size() - 1].type.size();
					else localVarOffset = 0;
				}
			}
			else if (kwtoken->checkFlag(KeywordFlags::If))
			{
				PossibleFunctionCalls call;
				call.isIf = true;

				call.codeIndex = (int)code.data.size();
				int ntIndex = findEndLine(ptoken);
				call.ntokenIndex = ntIndex;

				if (ntIndex + 1 < tokens.size() && reinterpret_cast<Keyword*>(tokens[ntIndex + 1])->checkFlag(KeywordFlags::Else))
				{
					call.elseNtokenIndex = findEndLine(tokens.begin() + ntIndex + 1);
				}

				functionCalls.push(call);
			}
			else if (kwtoken->checkFlag(KeywordFlags::While))
			{
				PossibleFunctionCalls call;
				call.isWhile = true;

				call.codeIndex = (int)code.data.size();
				int ntIndex = findEndLine(ptoken);
				call.ntokenIndex = ntIndex;
				call.tokenIndex = startLineIndex;

				functionCalls.push(call);
			}
			else if (kwtoken->checkFlag(KeywordFlags::LeftBracket))
			{
				functionCallsLevels.push(functionCalls.size());
			}
			else if (kwtoken->checkFlag(KeywordFlags::RightBracket))
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
						bool isOk = false;

						std::vector<typed_reg> args;
						int argsCount = reg.stackEmul.top().index;
						for (size_t i = 0; i < argsCount-call.index; i++)
							args.push_back(reg.free());
						for (size_t i = 0; i < args.size() / 2; i++)
							std::swap(args[i], args[args.size() - i - 1]);

						//правильно обработать ситуцию с разным количеством аргументов
						for (auto& f : call.functions)
						{
							for (size_t i = 0; i < args.size(); i++)
							{
								if (args[i].type.t != f->args[i].t &&
									f->args[i].t != makeTypeInfo<void>().t &&
									args[i].type.t != makeTypeInfo<nullptr_t>().t
									)
								{
									//WARNING(std::string("argument ") + std::to_string(i + 1) + ": "
									//	+ args[i].type.text() + " instead of " + f->args[i].text() + ".");
									goto cont;
								}
							}
							goto success;

						cont:
							continue;

						success:

							for (size_t i = 0; i < args.size(); i++)
								open_reg(args[i], f->args[i].ptr);
							code << opcode::call_cpp << call.index;
							for (size_t i = 0; i < sizeof(f->callStruct); i++)
								code << f->callStruct[i];
							isOk = true;
						}
			
						if (!isOk)
							throw Exception(std::string("All \"") + call.functions[0]->name + "\" overloads were insuitable.");
					}
				}

			}
			else if (kwtoken->checkFlag(KeywordFlags::New))
			{
				TypeInfo t = findType(*(ptoken+1));
				typed_reg& r = reg.alloc(t);
				code << opcode::alloc << r;
				code << (short)t.size();
				r.type.ptr++;

				bool isOk;
				PossibleFunctionCalls calls;
				calls.index = reg.stackEmul.top().index;
				for (auto& metadata : globalMetadataTable[t].members.get<FunctionInfo>())
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
			else if (kwtoken->checkFlag(KeywordFlags::This))
			{
				identifiersProcessor(ptoken);
			}
			else if (kwtoken->checkFlag(KeywordFlags::Null))
			{
				typed_reg& rn = reg.alloc(makeTypeInfo<nullptr_t>());
				code << (is_x64() ? opcode::push_64 : opcode::push_32) << rn << (void*)nullptr;
			}
			else
			{
				typed_reg r[2] = { typed_reg(), reg.free() };
				//std::swap(r[0], r[1]);


				for (auto& kw : keywords)
				{
					if (kw.arity == 1
						&& kwtoken == kw.text
						&& r[1].type.ptr >= kw.ptr[1]
						&& (r[1].type == kw.type[1] || kw.type[1] == makeTypeInfo<void>())
						)
					{
						open_reg(r[1], kw.ptr[1]);
						//open_reg(r[0], kw.ptr[0]);
						for (size_t i = 0; i < kw.opcodesCount; i++)
						{
							code << kw.opcodes[i];
							code << regindex_pair(r[1], r[1]);
						}
						//r[1].type = kw.rettype;
						reg.push(r[1]);
						goto break_ok;
					}
				}

				r[0] = reg.free();

				for (auto& kw : keywords)
				{
					if (kwtoken == kw.text
						&& r[0].type.ptr >= kw.ptr[0]
						&& r[1].type.ptr >= kw.ptr[1]
						&& (r[0].type == kw.type[0] || kw.type[0] == makeTypeInfo<void>() || 
							kw.type[1] == makeTypeInfo<nullptr_t>()
							)
						&& (r[1].type == kw.type[1] || kw.type[0] == makeTypeInfo<void>() ||
							kw.type[1] == makeTypeInfo<nullptr_t>()
							))
					{
						open_reg(r[1], kw.ptr[1]);
						open_reg(r[0], kw.ptr[0]);
						for (size_t i = 0; i < kw.opcodesCount; i++)
						{
							code << kw.opcodes[i];
							code << regindex_pair(kw.pairs[i].first == 0 ? r[0] : r[1], kw.pairs[i].second == 0 ? r[0] : r[1]);
						}

						r[0].type = kw.rettype;
						reg.push(r[0]);
						goto break_ok;
					}
				}

				throw Exception(std::string("Unknown operator \"") + kwtoken->value + "\" for types " + r[0].type.text() + " and " + r[1].type.text() + ".");
				break_ok:;
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
}
