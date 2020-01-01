#pragma once
#include "interpreter_util.h"
#include "Keyword.h"

namespace LizardScript
{
	SyntaxCore createDefaultSyntaxCore()
	{
		SyntaxCore core;
		core.textChars = { ':', '_', '@', '&' };
		core.breakChars = { '{', '}', '(', ')', '.', ';' };
		core.keywords = 
		{
			Keyword(_T("="), 20, Arity::Binary, SpecialKeywords::None, 0, Associativity::Right),
			Keyword(_T(">"), 30, Arity::Binary),
			Keyword(_T("<"), 30, Arity::Binary),
			Keyword(_T("+"), 50, Arity::Binary),
			Keyword(_T("*"), 60, Arity::Binary),
			Keyword(_T("-"), 50, Arity::Binary),
			Keyword(_T("/"), 60, Arity::Binary),

			Keyword(_T("++"), 90, Arity::Binary),
			Keyword(_T("--"), 90, Arity::Binary),

			Keyword(_T("("), 0, Arity::None, SpecialKeywords::LeftBracket),
			Keyword(_T(")"), 0, Arity::None, SpecialKeywords::RightBracket),
			Keyword(_T("{"), 0, Arity::None, SpecialKeywords::LeftBrace, KeywordFlags::EndLine),
			Keyword(_T("}"), 0, Arity::None, SpecialKeywords::RightBrace, KeywordFlags::EndLine),
			Keyword(_T(","), 0, Arity::None, SpecialKeywords::Comma),
			Keyword(_T("."), 0, Arity::None, SpecialKeywords::Dot, KeywordFlags::ParserAsNonOp),
			Keyword(_T(";"), 0, Arity::None, SpecialKeywords::None, KeywordFlags::EndLine),

			Keyword(_T("if"), 0, Arity::None, SpecialKeywords::If, KeywordFlags::ParserAsNonOp),
			Keyword(_T("while"), 0, Arity::None, SpecialKeywords::While, KeywordFlags::ParserAsNonOp),
			Keyword(_T("else"), 0, Arity::None, SpecialKeywords::Else, KeywordFlags::ParserAsNonOp),

			Keyword(_T("this"), 0, Arity::None, SpecialKeywords::This, KeywordFlags::ParserAsNonOp),
			Keyword(_T("null"), 0, Arity::None, SpecialKeywords::Null, KeywordFlags::ParserAsNonOp),

			Keyword(_T("new"), 0, Arity::None, SpecialKeywords::New, KeywordFlags::ParserAsNonOp),
			Keyword(_T("local"), 0, Arity::None, SpecialKeywords::NewLocal, KeywordFlags::ParserAsNonOp),

			Keyword(_T("class"), 0, Arity::None, SpecialKeywords::Class, KeywordFlags::ParserAsNonOp),
		};
		return core;
	}
}