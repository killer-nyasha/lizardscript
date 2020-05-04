#pragma once
#include "i_lexer.h"
#include "SyntaxCore.h"
#include "LsFunction.h"

namespace LizardScript
{
	class LsAsm
	{
		SyntaxCore createSyntaxCore();

		SyntaxCore core;

	public:
		LsAsm()
		{
			core = createSyntaxCore();
		}

		LsFunction assemble(const TCHAR* text, size_t length = 0);
	};
}