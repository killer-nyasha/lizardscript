#pragma once
#include "SyntaxCore.h"

namespace LizardScript
{
	class Parser
	{
	public:
		Parser(SyntaxCore& core, std::vector<TCHAR*>& tokens, std::vector<TCHAR*>& parserTokens, std::stack<Keyword*>& parserStack);

		//std::vector<TCHAR*> optimize(SyntaxCore& core, std::vector<TCHAR*>& tokens);
	};
}