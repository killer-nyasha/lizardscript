#include "stdafx.h"
#include "ILexer.h"
#include "Lexer.h"

namespace LizardScript
{
	PoolPointer<LexerData> runLexer(SyntaxCore& core, const TCHAR* text)
	{			
		return std::move(Lexer(core, text).run()); 
	}
}