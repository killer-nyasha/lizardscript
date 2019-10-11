#pragma once
#include "interpreter_util.h"

namespace LizardScript
{
	enum class KeywordAttributes
	{
		ParserKeyword,
		ParserIgnore,
		ParserNonKeyword,
	};

	struct KeywordFlags
	{
		enum //нужно ли??
		{
			None = 0,
			EndLine = 1,
			Comma = 1 << 1,
			LeftBracket = 1 << 2,
			RightBracket = 1 << 3,
			Dot = 1 << 4,
			If = 1 << 5,
			While = 1 << 6,
			LeftBrace = (1 << 7) | 1 /*| (1 << 2)*/,
			RightBrace = (1 << 8) | 1 /*| (1 << 3)*/,
			Else = 1 << 9,
			Var = 1 << 10,
			New = 1 << 11,
			This = 1 << 12,
			Null = 1 << 13
		} value;

		operator bool()
		{
			return value != 0;
		}

		bool operator==(KeywordFlags other)
		{
			return value == other.value;
		}

		bool operator!=(KeywordFlags other)
		{
			return value != other.value;
		}

		//KeywordFlags operator&(KeywordFlags second)
		//{
		//	return { (decltype(value))(value & second.value) };
		//}

		//KeywordFlags operator|(KeywordFlags second)
		//{
		//	return { (decltype(value))(value | second.value) };
		//}

		KeywordFlags(int f)
		{
			value = (decltype(value))f;
		}
	};




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

		KeywordFlags flags = 0;

		Keyword()
		{ }

		Keyword(const TCHAR* val)
		{
			_tcscpy(value, val);
		}

		Keyword(const TCHAR* val, int prioriry, int arity, KeywordFlags flags = 0, bool rightAssoc = false) 
			: priority(prioriry), arity(arity), isRightAssociative(rightAssoc), flags(flags)
		{

			_tcscpy(value, val);
		}

		bool operator==(const Keyword &kw) const { return _tcscmp(value, kw.value) == 0; }
		bool operator>=(const Keyword &kw) const { return _tcscmp(value, kw.value) >= 0; }
		bool operator<=(const Keyword &kw) const { return _tcscmp(value, kw.value) <= 0; }

		int checkFlag(KeywordFlags f)
		{
			return (int)flags.value & (int)f.value;
		}
	};
}