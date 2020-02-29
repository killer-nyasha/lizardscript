#include "pch.h"
#include "Parser.h"

namespace LizardScript
{
	void Parser::endLine()
	{
		while (!parserStack->empty())
		{
			parserTokens->push_back(parserStack->top()->value);
			parserStack->pop();
		}
	}

	bool Parser::popPredicate(const OperatorToken* operatorToken)
	{
		if (parserStack->empty())
			return false;

		if (parserStack->top()->type == KeywordTokenType::Unary || 
			parserStack->top()->type == KeywordTokenType::Binary)
		{
			const OperatorToken* otherOperator = reinterpret_cast<const OperatorToken*>(parserStack->top());

			if (operatorToken->associativity == Associativity::Left)
				return (operatorToken->priority < otherOperator->priority);
			else return operatorToken->priority <= otherOperator->priority;
		}
			
		return false;
			//&& !parserStack->top()->checkSpecial(SpecialKeywords::LeftBracket)
	}

	void Parser::endBrackets()
	{
		bool ok = false;
		
		while (parserStack->size() > 0 
			&& parserStack->top()->type != KeywordTokenType::LeftBracket
			)
		{
			parserTokens->push_back(parserStack->top()->value), parserStack->pop();
		}
		if (parserStack->size() > 0 && parserStack->top()->type == KeywordTokenType::LeftBracket)
		{
			ok = true; parserStack->pop();
		}

		if (!ok)
			throw Exception("Bracket wasn't closed");
	}

	Parser::Parser(const LexerData& lexerData)
		: lexerData(lexerData)
	{
		parserTokens->clear();
		while (parserStack->size() > 0)
			parserStack->pop();
	}

	//ссылка на LexerData???
	PoolPointer<std::vector<const void*>> Parser::run()
	{
		for (size_t i = 0; i < lexerData.tokens.size(); i++)
		{
			const void* token = lexerData[i];

			if (KeywordToken::isKeyword(token))
			{
				const KeywordToken* kwtoken = reinterpret_cast<const KeywordToken*>(token);

				if (kwtoken->type == KeywordTokenType::Simple)
				{
					if (kwtoken->parserFlags == ParserFlags::None)
					{

					}
					else if (kwtoken->parserFlags == ParserFlags::Comma)
					{
						while (parserStack->top()->type != KeywordTokenType::LeftBracket)
							parserTokens->push_back(parserStack->top()->value), parserStack->pop();
					}
					else if (kwtoken->parserFlags == ParserFlags::EndLine)
					{
						endLine();
					}

					parserTokens->push_back(token);
				}
				else if (kwtoken->type == KeywordTokenType::Unary ||
					kwtoken->type == KeywordTokenType::Binary)
				{
					const OperatorToken* operatorToken = OperatorToken::asOperator(kwtoken);

					while (popPredicate(operatorToken))
					{
						parserTokens->push_back(parserStack->top()->value);
						parserStack->pop();
					}
					parserStack->push(operatorToken);
				}
				else if (kwtoken->type == KeywordTokenType::LeftBracket)
				{
					parserStack->push(kwtoken);
					parserTokens->push_back(token);
				}
				else if (kwtoken->type == KeywordTokenType::LeftBracket)
				{
					endBrackets();
					parserTokens->push_back(token);
				}
			}
			else parserTokens->push_back(token);
		}

		endLine();

		return std::move(parserTokens);
	}

}