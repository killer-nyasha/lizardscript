#include "stdafx.h"
#include "SemanticCore.h"
#include "Pools.h"

using namespace LizardScript;

void SemanticCore::findOperator(KeywordToken* token)
{
	PoolPointer<std::vector<UnaryOperator*>> selectedUnary;
	PoolPointer<std::vector<UnaryOperator*>> selectedBinary;

	if (token->type == KeywordTokenType::Unary)
	{
		findUnary(OperatorToken::asOperator(token));
	}
	else if (token->type == KeywordTokenType::Binary)
	{
		findBinary(OperatorToken::asOperator(token));
	}
	else throw Exception();
}


void SemanticCore::findUnary(OperatorToken* token)
{
	for (auto& u : unary)
	{
		if (u->type == OperatorType::Override)
		{
			if (check())
		}
	}
}

void SemanticCore::findBinary(OperatorToken* token)
{

}