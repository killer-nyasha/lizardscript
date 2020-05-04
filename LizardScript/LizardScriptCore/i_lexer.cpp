#include "pch.h"
#include "i_lexer.h"
#include "Lexer.h"

namespace LizardScript
{
	PoolPointer<LexerData> runLexer(const SyntaxCore& core, const TCHAR* text, size_t length)
	{			
		return std::move(Lexer(core, text, length).run()); 
	}
}