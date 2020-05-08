/*!
\file LizardScriptDefault.h
\brief LizardScript compiler instance with default settings
\author killer-nyasha
\version 0.2
\date 17.01.2020
*/
#pragma once
#include "Exception.h"

namespace LizardScript
{
	class SyntaxCore;

	namespace Default
	{
		//!call it to create LizardScript compiler instance with default settings
		void init();

		extern SyntaxCore* syntaxCore;

		inline SyntaxCore& getSyntaxCore()
		{
			if (syntaxCore == nullptr)
				throw Exception("Default LizardScript objects weren't initialized!");
			return *syntaxCore;
		}
	}
}

