#include "stdafx.h"
#include "Parser.h"

namespace LizardScript
{
	Parser::Parser(SyntaxCore& core, std::vector<TCHAR*>& tokens, std::vector<TCHAR*>& parserTokens, std::stack<Keyword*>& parserStack)
	{
		for (TCHAR* token : tokens)
		{
			if (core.isKeyword(token))
			{
				Keyword* keywordToken = (Keyword*)token;
					
				if (keywordToken->checkFlag(KeywordFlags::ParserAsNonOp))//parser::asNonKeyword
				{
					parserTokens.push_back(token);
				}
				else if (keywordToken->checkSpecial(SpecialKeywords::Comma))
				{
					while (!parserStack.top()->checkSpecial(SpecialKeywords::LeftBracket))
						parserTokens.push_back(parserStack.top()->value), parserStack.pop();
				}
				else if (keywordToken->checkFlag(KeywordFlags::EndLine))
				{
					while (!parserStack.empty())
						parserTokens.push_back(parserStack.top()->value), parserStack.pop();
					parserTokens.push_back(token);
				}
				else if (keywordToken->checkSpecial(SpecialKeywords::LeftBracket))
				{
					parserTokens.push_back(token);
					parserStack.push(keywordToken);
				}
				else if (keywordToken->checkSpecial(SpecialKeywords::RightBracket))
				{
					//тут может вылететь!
					bool ok = false;
					while (parserStack.size() > 0 && !parserStack.top()->checkSpecial(SpecialKeywords::LeftBracket))
					{
						parserTokens.push_back(parserStack.top()->value), parserStack.pop();
					}
					if (parserStack.size() > 0 && parserStack.top()->checkSpecial(SpecialKeywords::LeftBracket))
					{
						ok = true; parserStack.pop();
					}

					if (!ok)
						throw Exception("Unclosed bracket");
						
					parserTokens.push_back(token);
				}
				else
				{
					while (!parserStack.empty()
						&&
						(keywordToken->priority < parserStack.top()->priority ||
						(keywordToken->associativity == Associativity::Left && keywordToken->priority == parserStack.top()->priority))
						&& !parserStack.top()->checkSpecial(SpecialKeywords::LeftBracket))
						parserTokens.push_back(parserStack.top()->value), parserStack.pop();
					parserStack.push(keywordToken);
				}
			}
			else parserTokens.push_back(token);
		}
		while (!parserStack.empty())
		{
			parserTokens.push_back(parserStack.top()->value), parserStack.pop();
		}
	}
}