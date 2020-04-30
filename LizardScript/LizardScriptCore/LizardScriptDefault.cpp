#include "pch.h"
#include <initializer_list> 

#include "LizardScriptDefault.h"

namespace LizardScript
{
	template <typename T, typename F>
	void set_vector(std::vector<T>& vector, std::initializer_list<F> f)
	{
		for (auto& a : f)
			vector.push_back(T(a));
	}

	namespace Default
	{
		SyntaxCore syntaxCore;

		SyntaxCore createSyntaxCore()
		{
			SyntaxCore core;
			core.textChars = { ':', '_', '@', '&' };
			core.breakChars = { '{', '}', '(', ')', '.', ';' };

			//new OperatorToken(".", KeywordTokenType::Binary, 100),
			//	new OperatorToken(",", KeywordTokenType::Binary, 10),

			//	new BracketToken(_T("("), true),
			//	new BracketToken(_T(")"), false),

			set_vector<std::unique_ptr<OperatorToken>, OperatorToken*>(core.prefixUnary,
			{
				new OperatorToken("-", KeywordTokenType::PrefixUnary, 70),
			});

			set_vector<std::unique_ptr<OperatorToken>, OperatorToken*>(core.postfixUnary,
			{
				new OperatorToken("-", KeywordTokenType::PostfixUnary, 60),
				new OperatorToken("--", KeywordTokenType::PostfixUnary, 60),
			});

			set_vector<std::unique_ptr<OperatorToken>, OperatorToken*>(core.binaryOperators,
			{
				new OperatorToken("=", KeywordTokenType::Binary, 20, Associativity::Right),

				new OperatorToken(">", KeywordTokenType::Binary, 30),
				new OperatorToken("<", KeywordTokenType::Binary, 30),

				new OperatorToken("+", KeywordTokenType::Binary, 50),
				new OperatorToken("-", KeywordTokenType::Binary, 50),
				new OperatorToken("*", KeywordTokenType::Binary, 50),
				new OperatorToken("/", KeywordTokenType::Binary, 50),

				new OperatorToken("--", KeywordTokenType::Binary, 50),

				//new Keyword(_T("="), 20, Arity::Binary, SpecialKeywords::None, 0, Associativity::Right),
				//Keyword(_T(">"), 30, Arity::Binary),
				//Keyword(_T("<"), 30, Arity::Binary),
				//Keyword(_T("+"), 50, Arity::Binary),
				//Keyword(_T("*"), 60, Arity::Binary),
				//Keyword(_T("-"), 50, Arity::Binary),
				//Keyword(_T("/"), 60, Arity::Binary),

				//Keyword(_T("++"), 90, Arity::Binary),
				//Keyword(_T("--"), 90, Arity::Binary),



				//Keyword(_T("{"), 0, Arity::None, SpecialKeywords::LeftBrace, KeywordFlags::EndLine),
				//Keyword(_T("}"), 0, Arity::None, SpecialKeywords::RightBrace, KeywordFlags::EndLine),
				//Keyword(_T(","), 0, Arity::None, SpecialKeywords::Comma),
				//Keyword(_T("."), 0, Arity::None, SpecialKeywords::Dot, KeywordFlags::ParserAsNonOp),
				//Keyword(_T(";"), 0, Arity::None, SpecialKeywords::None, KeywordFlags::EndLine),

				//Keyword(_T("if"), 0, Arity::None, SpecialKeywords::If, KeywordFlags::ParserAsNonOp),
				//Keyword(_T("while"), 0, Arity::None, SpecialKeywords::While, KeywordFlags::ParserAsNonOp),
				//Keyword(_T("else"), 0, Arity::None, SpecialKeywords::Else, KeywordFlags::ParserAsNonOp),

				//Keyword(_T("this"), 0, Arity::None, SpecialKeywords::This, KeywordFlags::ParserAsNonOp),
				//Keyword(_T("null"), 0, Arity::None, SpecialKeywords::Null, KeywordFlags::ParserAsNonOp),

				//Keyword(_T("new"), 0, Arity::None, SpecialKeywords::New, KeywordFlags::ParserAsNonOp),
				//Keyword(_T("local"), 0, Arity::None, SpecialKeywords::NewLocal, KeywordFlags::ParserAsNonOp),

				//Keyword(_T("class"), 0, Arity::None, SpecialKeywords::Class, KeywordFlags::ParserAsNonOp),
			});

			core.confirmChanges();

			return core;
		}

		void init()
		{
			syntaxCore = createSyntaxCore();
		}

	}
}