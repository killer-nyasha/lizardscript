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
			Keyword(_T("="), 20, 2, 0, true),
			Keyword(_T(">"), 30, 2),
			Keyword(_T("<"), 30, 2),
			Keyword(_T("+"), 50, 2),
			Keyword(_T("*"), 60, 2),
			Keyword(_T("-"), 50, 2),
			Keyword(_T("/"), 60, 2),

			Keyword(_T("++"), 90, 2),
			Keyword(_T("--"), 90, 2),

			Keyword(_T("("), 0, 0, KeywordFlags::LeftBracket),
			Keyword(_T(")"), 0, 0, KeywordFlags::RightBracket),
			Keyword(_T("{"), 0, 0, KeywordFlags::LeftBrace),
			Keyword(_T("}"), 0, 0, KeywordFlags::RightBrace),
			Keyword(_T(","), 0, 0, KeywordFlags::Comma),
			Keyword(_T("."), 0, 0, KeywordFlags::Dot),
			Keyword(_T(";"), 0, 0, KeywordFlags::EndLine),
			Keyword(_T("if"), 0, 0, KeywordFlags::If),
			Keyword(_T("while"), 0, 0, KeywordFlags::While),
			Keyword(_T("else"), 0, 0, KeywordFlags::Else),

			Keyword(_T("this"), 0, 0, KeywordFlags::This),
			Keyword(_T("null"), 0, 0, KeywordFlags::Null),

			//Keyword(_T("$"), 0, 0, KeywordFlags::Var),
			//Keyword(_T("let"), 0, 0, KeywordFlags::Var),

			Keyword(_T("new"), 0, 0, KeywordFlags::New),
		};
		return core;
	}
}