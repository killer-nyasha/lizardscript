#include "stdafx.h"
#include "ILexer.h"
#include "Lexer.h"

namespace LizardScript
{
	LexerData* runLexer(SyntaxCore& core, const TCHAR* text, bool multiThread)
	{
		Lexer lxr = Lexer(core, text, multiThread);
		return lxr.run();
	}
}