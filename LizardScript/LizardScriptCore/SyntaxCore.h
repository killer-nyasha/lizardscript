/*!
\file SyntaxCore.h
\brief SyntaxCore class.
\author killer-nyasha
\version 0.2
\date 17.01.2020
*/
#pragma once
#include <vector>
#include <memory>

#include "interpreter_util.h"
#include "Tokens.h"

namespace LizardScript
{
	//!Contains basic inforamtion about all keywords and special chars.
	class SyntaxCore
	{
	public:
		//!Non-text chars which compiler reads as they were letters
		std::vector<TCHAR> textChars;

		//!Symbols which always split text into different tokens.
		//!for example, `((` - two tokens `(` and `(` unlike `++` - one token.
		std::vector<TCHAR> breakChars;

		//!all keywords
		//std::vector<std::unique_ptr<KeywordToken>> keywords;

		//!prefix unary operators
		std::vector<std::unique_ptr<OperatorToken>> prefixUnary;

		//!postfix unary operators
		std::vector<std::unique_ptr<OperatorToken>> postfixUnary;

		//!binary operators
		std::vector<std::unique_ptr<OperatorToken>> binaryOperators;

		//!binary operators
		std::vector<std::unique_ptr<KeywordToken>> simpleKeywords;

		SyntaxCore() 
		{ 
			
		}

		//!deletes all keywords
		//~SyntaxCore()
		//{
		//	for (auto& k : keywords)
		//		delete k;
		//}

		//!you have to call this function after any changes in this SyntaxCore before using it in compiler
		void confirmChanges();
	};

	template <typename T, typename F>
	inline void set_vector(std::vector<T>& vector, std::initializer_list<F> f)
	{
		for (auto& a : f)
			vector.push_back(T(a));
	}
}