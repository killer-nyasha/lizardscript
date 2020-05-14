#include "pch.h"
#include "Parser.h"

namespace LizardScript
{
	void Parser::endLine()
	{
		while (!parserStack->empty())
		{
			parserTokens->push_back(parserStack->top());
			parserStack->pop();
		}
	}

	bool Parser::popPredicate(OperatorToken* operatorToken)
	{
		if (parserStack->empty())
			return false;

		if (parserStack->top()->is_operator())
		{
			const OperatorToken* otherOperator = reinterpret_cast<const OperatorToken*>(parserStack->top());

			if (operatorToken->associativity == Associativity::Left)
				return (operatorToken->priority <= otherOperator->priority);
			else 
				return operatorToken->priority < otherOperator->priority;
		}
			
		return false;
	}

	void Parser::endBrackets()
	{
		bool ok = false;
		
		while (parserStack->size() > 0 
			&& parserStack->top()->type != KeywordTokenType::LeftBracket
			)
		{
			parserTokens->push_back(parserStack->top()), parserStack->pop();
		}
		if (parserStack->size() > 0 && parserStack->top()->type == KeywordTokenType::LeftBracket)
		{
			ok = true; parserStack->pop();
		}

		if (!ok)
			throw Exception("Bracket wasn't closed");
	}

	Parser::Parser(LexerData& lexerData)
		: lexerData(lexerData)
	{
		parserTokens->clear();
		while (parserStack->size() > 0)
			parserStack->pop();
	}

	PoolPointer<std::vector<void*>> Parser::run()
	{
		for (size_t i = 0; i < lexerData.tokens->size(); i++)
		{
			KeywordToken* kwtoken;

			if (lexerData.tryGetKeyword(i, kwtoken))
			{
				if (kwtoken->type == KeywordTokenType::Simple)
				{
					if (kwtoken->parserFlags == ParserFlags::None)
					{

					}
					else if (kwtoken->parserFlags == ParserFlags::Comma)
					{
						while (parserStack->top()->type != KeywordTokenType::LeftBracket)
							parserTokens->push_back(parserStack->top()), parserStack->pop();
					}
					else if (kwtoken->parserFlags == ParserFlags::EndLine)
					{
						endLine();
					}

					parserTokens->push_back(kwtoken);
				}
				else if (kwtoken->is_operator())
				{
					OperatorToken* operatorToken = OperatorToken::asOperator(kwtoken);

					while (popPredicate(operatorToken))
					{
						parserTokens->push_back(parserStack->top());
						parserStack->pop();
					}
					parserStack->push(operatorToken);
				}
				else if (kwtoken->type == KeywordTokenType::LeftBracket)
				{
					parserStack->push(kwtoken);
					parserTokens->push_back(kwtoken);
				}
				else if (kwtoken->type == KeywordTokenType::RightBracket)
				{
					endBrackets();
					parserTokens->push_back(kwtoken);
				}
			}
			else
			{
				TCHAR* t; size_t tIndex;
				if (lexerData.tryGetValue(i, t, &tIndex))
					parserTokens->push_back(reinterpret_cast<void*>(tIndex));
				else throw Exception();
			}
				
		}

		endLine();

		return std::move(parserTokens);
	}

}