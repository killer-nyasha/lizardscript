#pragma once
#include "Tokens.h"
#include "Operators.h"

namespace LizardScript
{
	class SemanticCore
	{
		std::vector<UnaryOperator*> unary;
		std::vector<BinaryOperator*> binary;
		std::vector<UnaryOperator*> casts;

		void findUnary(OperatorToken* token);
		void findBinary(OperatorToken* token);

	public:

		void findOperator(KeywordToken* token);

	};
}