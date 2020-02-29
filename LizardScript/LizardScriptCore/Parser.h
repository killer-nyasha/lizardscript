/*!
\file Parser.h
\brief Converts infix notation to postfix
\author killer-nyasha
\version 0.2
\date 29.02.2020
*/
#pragma once
#include "SyntaxCore.h"
#include "Pools.h"
#include "i_lexer.h"

namespace LizardScript
{
	class Parser
	{
		void endLine();
		void endBrackets();
		bool popPredicate(const OperatorToken* operatorToken);

		const LexerData& lexerData;
		//const std::vector<void*>& lexerTokens;
		PoolPointer<std::stack<const KeywordToken*>> parserStack;
		PoolPointer<std::vector<const void*>> parserTokens;

	public:

		Parser(const LexerData& tokens);

		PoolPointer<std::vector<const void*>> run();

		//std::vector<TCHAR*> optimize(SyntaxCore& core, std::vector<TCHAR*>& tokens);
	};
}