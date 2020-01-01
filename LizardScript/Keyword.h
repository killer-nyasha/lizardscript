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

	enum class Arity
	{
		None,
		Unary,
		Binary
	};

	enum class Associativity
	{
		None,
		Left,
		Right
	};

	//!\warning There aren't any differences between "keywords" and "operators" in expression language
	struct Keyword
	{
		//!text representation of Keyword
		TCHAR value[16];

		//!by default '+' and '-' has priority 50, '*' and '/' - 70
		int priority;

		//deprecated
		Arity arity;

		Associativity associativity;

	private:

		SpecialKeywords special;
		int flags = 0;

	public:

		Keyword()
		{ }

		Keyword(const TCHAR* val)
		{
			_tcscpy(value, val);
		}

		Keyword(const TCHAR* val, 
			int prioriry, 
			Arity arity, 
			SpecialKeywords special = SpecialKeywords::None, 
			int flags = 0, 
			Associativity associativity = Associativity::Left)
			: priority(prioriry), arity(arity), associativity(associativity), flags(flags), special(special)
		{

			_tcscpy(value, val);
		}

		bool operator==(const Keyword &kw) const 
		{ 
			return _tcscmp(value, kw.value) == 0 && arity == kw.arity; 
		}
		bool operator!=(const Keyword& kw) const
		{
			return _tcscmp(value, kw.value) != 0 && arity != kw.arity;
		}
		bool operator>=(const Keyword &kw) const 
		{
			int c = _tcscmp(value, kw.value);
			if (c > 0)
				return true;
			else if (c == 0)
				return arity >= kw.arity;
			else return false;
		}
		bool operator<=(const Keyword &kw) const 
		{ 
			int c = _tcscmp(value, kw.value);
			if (c < 0)
				return true;
			else if (c == 0)
				return arity <= kw.arity;
			else return false;
		}
		bool operator>(const Keyword& kw) const
		{
			int c = _tcscmp(value, kw.value);
			if (c < 0)
				return true;
			else if (c == 0)
				return arity > kw.arity;
			else return false;
		}
		bool operator<(const Keyword& kw) const
		{
			int c = _tcscmp(value, kw.value);
			if (c < 0)
				return true;
			else if (c == 0)
				return arity < kw.arity;
			else return false;
		}

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