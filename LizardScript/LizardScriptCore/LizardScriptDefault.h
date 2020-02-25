/*!
\file LizardScriptDefault.h
\brief LizardScript compiler instance with default settings
\author killer-nyasha
\version 0.2
\date 17.01.2020
*/
#pragma once
#include "SyntaxCore.h"

namespace LizardScript
{
	namespace Default
	{
		//!call it to create LizardScript compiler instance with default settings
		void init();

		extern SyntaxCore syntaxCore;
	}
}

