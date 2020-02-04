#pragma once
#include "ByteCodeGenerator.h"
#include "FindType.h"

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

		auto nkwtoken = reinterpret_cast<KeywordToken*>(*nptoken);
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

