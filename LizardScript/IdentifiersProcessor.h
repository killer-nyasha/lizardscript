#pragma once
#include "FindType.h"

template <typename C>
class reverse_wrapper
{
	C& container;

public:
	reverse_wrapper(C& container) : container(container)
	{ }

	auto begin()
	{
		return container.rbegin();
	}

	auto end()
	{
		return container.rend();
	}
};

template <typename C>
auto make_reverse_wrapper(C& container)
{
	return reverse_wrapper<C>(container);
}

//#define make_reverse_wrapper

void ByteCodeGenerator::identifiersProcessor(std::vector<TCHAR*>::iterator& ptoken)
{
	TCHAR* token = *ptoken;
	typed_reg& rn = reg.alloc(type.withPtr(1));
	//rn.type.ptr++;
	int tIndex = ptoken - tokens.begin();

	bool thisPushed = false;
	bool exit = false;

	if (core.isKeyword(token) && ((KeywordToken*)token)->checkSpecial(SpecialKeywords::This))
	{
		if (core.isKeyword(*(ptoken + 1)) && ((KeywordToken*)*(ptoken + 1))->checkSpecial(SpecialKeywords::Dot))
		{
			thisPushed = true;
			ptoken += 2;
		}
		else exit = true;
	}
	token = *ptoken;

	int pushThisOpcodeIndex = code.data.size();
	code << opcode::push_this;
	code << rn;

	TypeInfo currentType = type;

	bool setThisPushed = false;

	if (exit)
		return;

	while (
		true
		//((ptoken) >= tokens.begin()) && ((Keyword*)*(ptoken + 1))->checkFlag(KeywordFlags::Dot) && ((token = *ptoken))
		)
	{
		bool isOk = false;
		short int offset = 0;

		if (!thisPushed)
		for (size_t i = localVar.size()-1; i + 1 >= 1; i--)
		{
			if (_tcscmp(&localVar[i].name[0], token) == 0)
			{
				//localVarAddr.push_back({ (int)code.data.size()+2, token });
				offset = localVar[i].offset;
				rn.type = localVar[i].type;
				rn.type.ptr++;
				code.data[pushThisOpcodeIndex] = (char)opcode::push_stackbase;
				isOk = true; break;
			}
		}

		if (!isOk)
		{
			auto& metatable = globalMetadataTable[currentType];

			for (auto& metadata : make_reverse_wrapper(metatable.members.get<FieldInfo>()))
			{
				if (_tcscmp(&metadata.name[0], token) == 0)
				{
					offset = metadata.offset;
					rn.type = metadata.type;
					rn.type.ptr++;
					isOk = true; break;
				}
			}
		}
		if (!isOk)
		{
			auto& metatable = globalMetadataTable[currentType];

			if (setThisPushed)
			{
				//undo set this
				code.data.resize(code.data.size()-2);
			}

			PossibleFunctionCalls calls;
			calls.index = reg.stackEmul.top().index;

			for (auto& metadata : make_reverse_wrapper(metatable.members.get<FunctionInfo>()))
			{
				if (_tcscmp(&metadata.name[0], token) == 0)
				{
					calls.functions.push_back(&metadata);
					isOk = true;
				}
			}
			if (isOk)
				functionCalls.push(calls);
		}
		if (!isOk)
			if (_tcscmp("lsl", &token[0]) == 0)
			{
				//undo push this
				code.data[pushThisOpcodeIndex] = (char)opcode::push_lsl;

				for (auto& metatable : globalMetadataTable)
				{
					if (_tcscmp(&metatable.first.text()[0], "LizardScript::LizardScriptLibrary") == 0)
					{
						rn.type = metatable.first;
						rn.type.ptr++;
						isOk = true;
					}
				}
			}

		if (!isOk)
			{
				if (tIndex + 1 >= tokens.size())
					throw Exception("Missing local variable name.");

				//undo push_this
				reg.free(); code.data.resize(code.data.size() - 2);
				newLocalVariable(*(ptoken), *(ptoken + 1));
				ptoken += 1;
			}

			//else throw Exception(std::string("Unknown identifier \"") + currentType.text() + "::" + token + "\".");

		//ok1:
		if (offset != 0)
		{
			code << opcode::push_offset;
			code << rn;
			code << offset;
		}

		if (  core.isKeyword(*(ptoken + 1)) &&
			((KeywordToken*)*(ptoken + 1))->checkSpecial(SpecialKeywords::Dot)
			&& ((ptoken += 2) >= tokens.begin()) && ((token = *ptoken))
			)
		{
			if (rn.type.ptr > 1)
			{
				setThisPushed = true;
				//если функция, то вот это вот откатываем?? или нет??
				code << opcode::set_this;
				code << rn;
			}
			currentType = rn.type;
		}
		else break;
	}
	//currentType = type;
}