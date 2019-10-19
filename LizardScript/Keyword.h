#pragma once
#include "interpreter_util.h"

namespace LizardScript
{
	enum class KeywordAttributes
	{
		ParserKeyword,
		ParserIgnore,
		ParserNonKeyword,
		EndLine,
	};

	enum class SpecialKeywords
	{
		None = 0,
		Comma,
		LeftBracket,
		RightBracket,
		Dot,
		If,
		While,
		LeftBrace,
		RightBrace,
		Else,
		Var,
		New,
		This,
		Null,
		Class,
		NewLocal,
	};

	namespace KeywordFlags
	{
		enum Flags
		{
			None = 0,
			ParserAsNonOp = 1,
			EndLine = 2,
		};
	}

	//!\warning There aren't any differences between "keywords" and "operators" in expression language
	struct Keyword
	{
		//!text representation of Keyword
		TCHAR value[32];

		//!by default '+' and '-' has priority 50, '*' and '/' - 70
		int priority;

		//deprecated
		int arity;

		bool isRightAssociative;

		SpecialKeywords special;
		/*KeywordFlags::Flags*/int flags = 0;

		Keyword()
		{ }

		Keyword(const TCHAR* val)
		{
			_tcscpy(value, val);
		}

		Keyword(const TCHAR* val, int prioriry, int arity, SpecialKeywords special = SpecialKeywords::None, int flags = 0, bool rightAssoc = false) 
			: priority(prioriry), arity(arity), isRightAssociative(rightAssoc), flags(flags), special(special)
		{

			_tcscpy(value, val);
		}

		bool operator==(const Keyword &kw) const { return _tcscmp(value, kw.value) == 0; }
		bool operator>=(const Keyword &kw) const { return _tcscmp(value, kw.value) >= 0; }
		bool operator<=(const Keyword &kw) const { return _tcscmp(value, kw.value) <= 0; }

		bool checkFlag(int f)
		{
			return (f & flags) != 0;
		}

		bool checkSpecial(SpecialKeywords kw)
		{
			return special == kw;
		}
	};
}