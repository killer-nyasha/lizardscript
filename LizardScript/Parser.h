#pragma once
#include "SyntaxCore.h"
#include "Pools.h"

namespace LizardScript
{
	class Parser
	{
		void endLine();
		void endBrackets();
		bool popPredicate(OperatorToken* operatorToken);

	public:

		const std::vector<TCHAR*>& tokens;
		PoolPointer<std::vector<TCHAR*>> parserTokens;
		PoolPointer<std::stack<KeywordToken*>> parserStack;

		Parser(const std::vector<TCHAR*>& tokens);

		void run();


		//std::vector<TCHAR*> optimize(SyntaxCore& core, std::vector<TCHAR*>& tokens);
	};
}