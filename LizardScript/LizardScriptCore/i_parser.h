#pragma once
#include <vector>

#include "crossplatform_tchar.h"
#include "Pools.h"
#include "i_lexer.h"

namespace LizardScript
{
	//!Converts infix sequence of tokens to postfix notation.
	//!Used simplified shunting-yard algorithm.
	//!\returns pointer to vector of tokens in postfix notation. you have to free it manually.
	//!\param[in] r LexerData::tokens
	PoolPointer<std::vector<const void*>> runParser(const LexerData& lexerData);
}