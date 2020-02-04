#pragma once
#include "interpreter_util.h"

namespace LizardScript
{
	//!Converts infix sequence of tokens to postfix notation.
	//!Used simplified shunting-yard algorithm.
	//!\returns pointer to vector of tokens in postfix notation. you have to free it manually.
	//!\param[in] r LexerData::tokens
	std::vector<TCHAR*>* runParser(std::vector<TCHAR*>& r);
}