#pragma once
#include "SyntaxCore.h"
#include "LizardScriptDefault.h"
#include "LsFunction.h"

namespace LizardScript
{
	class LsCompiler
	{
		SyntaxCore& core;

	public:
		LsCompiler(SyntaxCore& core) : core(core)
		{
			
		}

		LsCompiler() :core(Default::getSyntaxCore())
		{

		}

		LsFunction compile(const TCHAR* text, size_t length = 0);

	};
}