#pragma once
#include "interpreter_util.h"

namespace LizardScript
{
	class SyntaxCore;

	struct LexerData
	{
		std::vector<TCHAR> values;
		std::vector<TCHAR*> tokens;
	};

	//class ILexer
	//{
	//	ILexer(SyntaxCore& core, const TCHAR* text, bool multiThread = false)
	//	{ }

	//	virtual ~ILexer() = 0;

	//	virtual LexerData* run() = 0;
	//};

	LexerData* runLexer(SyntaxCore& core, const TCHAR* text, bool multiThread = false);
}