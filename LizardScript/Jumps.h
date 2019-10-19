#pragma once
#include "ByteCodeGenerator.h"

using namespace LizardScript;

void ByteCodeGenerator::processJumps(int tIndex)
{
	for (size_t i = 0; i < fjmps.size(); i++)
	{
		auto& j = fjmps[i];
		if (tIndex == j.fromTokenIndex)
		{
			code << opcode::jmp << (char)0;
			jmps.push_back(jmp{ (int)code.data.size(), j.toTokenIndex, -1 });
			code << (int)0;
			fjmps.erase(fjmps.begin() + i); i--;
		}
	}
	for (size_t i = 0; i < jmps.size(); i++)
	{
		auto& j = jmps[i];
		if (tIndex == j.toTokenIndex)
		{
			if (j.toCodeBackIndex != -1)
			{
				code << opcode::jmp << (char)0 << j.toCodeBackIndex;
			}
			*reinterpret_cast<int*>(&code.data[j.codeIndex]) = code.data.size();
			jmps.erase(jmps.begin() + i); i--;
		}
	}
}

int ByteCodeGenerator::findEndLine(std::vector<TCHAR*>::iterator ptoken)
{
	int k = 1;
	for (auto nptoken = ptoken; nptoken < tokens.end(); nptoken++)
	{
		if (!core.isKeyword(*nptoken))
			continue;

		auto nkwtoken = reinterpret_cast<Keyword*>(*nptoken);
		int ntIndex = nptoken - tokens.begin();

		if (nkwtoken->checkSpecial(SpecialKeywords::LeftBrace))
			k++;
		if (nkwtoken->checkSpecial(SpecialKeywords::RightBrace))
		{
			k--;
			if (k == 1)
				return ntIndex;
		}
		if ((nkwtoken->checkFlag(KeywordFlags::EndLine) && (k == 1)) /*|| (k==0)*/)
			return ntIndex;
	}
	throw Exception();//?????
}

//==========================================================================
//offtopic))

bool ByteCodeGenerator::open_reg(typed_reg& r, int ptrLevel)
{
	
	while (r.type.ptr > ptrLevel)
	{
		if (r.type.size() <= 4)
			code << opcode::get_32 << r;
		else if (r.type.size() <= 8)
			code << opcode::get_64 << r;
		r.type.ptr--;

		if (r.type.size() > sizeof(void*))
		{
			r.type.ptr++;
			throw Exception(std::string("Type ") + r.type.text() + " can't be placed in a register.");
			return false;
		}
	}
	return true;
}