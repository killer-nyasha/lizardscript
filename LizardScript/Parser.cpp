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
						
						//TCHAR* callOp = (TCHAR*)"_call";
						parserTokens.push_back(token);
					}
					else
					{
						while (!parserStack.empty()
							&&
							(keywordToken->priority < parserStack.top()->priority ||
							(!keywordToken->isRightAssociative && keywordToken->priority == parserStack.top()->priority))
							&& !parserStack.top()->checkSpecial(SpecialKeywords::LeftBracket))
							parserTokens.push_back(parserStack.top()->value), parserStack.pop();
						parserStack.push(keywordToken);
					}
				}
				//else if (token[0] == '@')
				//{
				//	parserStack.push((Keyword*)token);
				//}
				else parserTokens.push_back(token);
			}
			while (!parserStack.empty())
			{
				parserTokens.push_back(parserStack.top()->value), parserStack.pop();
			}

			//оптимизация!!

			//if (false)
			//for (size_t i = 0; i < parserTokens.size(); i++)
			//{
			//	if (i+2 < parserTokens.size() 
			//		&& _tcscmp(parserTokens[i], "1") == 0
			//		&& _tcscmp(parserTokens[i], "+") == 0
			//		&& _tcscmp(parserTokens[i], "=") == 0
			//		)
			//	{

			//	}


			//	size_t nmStart = i;
			//	size_t nmEnd = i;

			//	while (nmEnd < parserTokens.size() && (!core.isKeyword(parserTokens[nmEnd]) || reinterpret_cast<Keyword*>(parserTokens[nmEnd])->checkFlag(KeywordFlags::Dot)))
			//		nmEnd++;

			//	if ((nmStart < nmEnd) && ((nmEnd - nmStart) % 2 == 0))
			//	{
			//		size_t nmMiddle = nmStart + (nmEnd - nmStart) / 2;

			//		for (size_t delta = 0; delta < nmMiddle - nmStart; delta++)
			//		{
			//			if (_tcscmp(parserTokens[nmStart + delta], parserTokens[nmMiddle + delta]) != 0)
			//				goto endc;
			//		}

			//		while (nmMiddle < nmEnd)
			//		{ parserTokens[nmMiddle] = (TCHAR*)_T("_dup"); nmMiddle++; } 
			//	}
			//endc:;
			//}

			//
		}

		//std::vector<TCHAR*> optimize(SyntaxCore& core, std::vector<TCHAR*>& tokens)
		//{
		//	std::deque<TCHAR*> parserStack = std::deque<TCHAR*>();
		//	std::vector<TCHAR*> parserTokens = std::vector<TCHAR*>();

		//	size_t stackSize = 0;

		//	for (TCHAR* token : tokens)
		//	{
		//		if (core.isKeyword(token))
		//		{
		//			Keyword* keywordToken = (Keyword*)token;

		//			//while (parserStack.size() >= keywordToken->arity)

		//			int argsCount = keywordToken->arity;

		//			while (stackSize > 0)
		//			{ argsCount--; stackSize--; }

		//			for (size_t i = 0; i < argsCount; i++)
		//				parserTokens.push_back(parserStack.front()), parserStack.pop_front();
		//			parserTokens.push_back(token);
		//			stackSize++;
		//		}
		//		else
		//		{
		//			parserStack.push_back(token); //stackSize++;
		//		}
		//	}

		//	return parserTokens;
		//}
}