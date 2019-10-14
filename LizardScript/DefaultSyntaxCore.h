#pragma once
#include "interpreter_util.h"
#include "Keyword.h"

namespace LizardScript
{
	SyntaxCore createDefaultSyntaxCore()
	{
		SyntaxCore core;
		core.textChars = { ':', '_', '@' };
		core.breakChars = { '{', '}', '(', ')', '.', ';' };
		core.keywords = 
		{
			Keyword(_T("="), 20, 2, SpecialKeywords::None, 0, true),
			Keyword(_T(">"), 30, 2),
			Keyword(_T("<"), 30, 2),
			Keyword(_T("+"), 50, 2),
			Keyword(_T("*"), 60, 2),
			Keyword(_T("-"), 50, 2),
			Keyword(_T("/"), 60, 2),

			Keyword(_T("++"), 90, 2),
			Keyword(_T("--"), 90, 2),

			Keyword(_T("("), 0, 0, SpecialKeywords::LeftBracket),
			Keyword(_T(")"), 0, 0, SpecialKeywords::RightBracket),
			Keyword(_T("{"), 0, 0, SpecialKeywords::LeftBrace, KeywordFlags::EndLine),
			Keyword(_T("}"), 0, 0, SpecialKeywords::RightBrace, KeywordFlags::EndLine),
			Keyword(_T(","), 0, 0, SpecialKeywords::Comma),
			Keyword(_T("."), 0, 0, SpecialKeywords::Dot, KeywordFlags::ParserAsNonOp),
			Keyword(_T(";"), 0, 0, SpecialKeywords::None, KeywordFlags::EndLine),

			Keyword(_T("if"), 0, 0, SpecialKeywords::If, KeywordFlags::ParserAsNonOp),
			Keyword(_T("while"), 0, 0, SpecialKeywords::While, KeywordFlags::ParserAsNonOp),
			Keyword(_T("else"), 0, 0, SpecialKeywords::Else, KeywordFlags::ParserAsNonOp),

			Keyword(_T("this"), 0, 0, SpecialKeywords::This, KeywordFlags::ParserAsNonOp),
			Keyword(_T("null"), 0, 0, SpecialKeywords::Null, KeywordFlags::ParserAsNonOp),

			//Keyword(_T("$"), 0, 0, KeywordFlags::Var, KeywordFlags::ParserAsNonOp),
			//Keyword(_T("let"), 0, 0, KeywordFlags::Var),

			Keyword(_T("new"), 0, 0, SpecialKeywords::New, KeywordFlags::ParserAsNonOp),

			Keyword(_T("class"), 0, 0, SpecialKeywords::Class, KeywordFlags::ParserAsNonOp),
		};
		return core;
	}
}