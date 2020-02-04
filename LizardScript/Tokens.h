#pragma once
#include "interpreter_util.h"

namespace LizardScript
{
	enum class ParserFlags
	{
		None = 0,
		Comma,
		EndLine
	};

	enum class Arity
	{
		Unary,
		Binary
	};

	enum class Associativity
	{
		Left,
		Right
	};

	enum class KeywordTokenType
	{
		Simple,
		Unary,
		Binary,
		LeftBracket,
		RightBracket,
		Override
	};
	
	bool kwtype_listA(KeywordTokenType type)
	{
		return type == KeywordTokenType::Simple
			|| type == KeywordTokenType::Unary
			|| type == KeywordTokenType::LeftBracket;
	}

	bool kwtype_listB(KeywordTokenType type)
	{
		return type == KeywordTokenType::Binary
			|| type == KeywordTokenType::LeftBracket
			|| type == KeywordTokenType::RightBracket;
	}

	bool kwtype_before_listA(KeywordTokenType type)
	{
		return type == KeywordTokenType::Unary
			|| type == KeywordTokenType::Binary
			|| type == KeywordTokenType::LeftBracket;
	}

	bool kwtype_before_listB(KeywordTokenType type)
	{
		return type == KeywordTokenType::Simple
			|| type == KeywordTokenType::Unary
			|| type == KeywordTokenType::RightBracket;
	}

	class KeywordToken
	{
#define IKEYWORD_SIGNATURE ('~'*256*256*256 + 'k'*256*256 + 'w'*256 + '~')
		int signature = IKEYWORD_SIGNATURE;
		
	public:

		TCHAR value[16];

		KeywordTokenType type = KeywordTokenType::Simple;
		ParserFlags parserFlags = ParserFlags::None;

		KeywordToken(const TCHAR* cvalue)
		{
			static_assert(offsetof(KeywordToken, signature) == 0, "pizda");
			_tcscpy(value, cvalue);
		}

		static inline bool isKeyword(const TCHAR* token)
		{
			return reinterpret_cast<const KeywordToken*>(token)->signature == IKEYWORD_SIGNATURE;
		}

		//virtual void parse() { };
		//virtual void action() { };

		bool operator==(const KeywordToken &kw) const 
		{ 
			return _tcscmp(value, kw.value) == 0; 
		}

		bool operator<(const KeywordToken& kw) const
		{
			return _tcscmp(value, kw.value) < 0;
		}

		bool operator>=(const KeywordToken& kw) const
		{
			return _tcscmp(value, kw.value) >= 0;
		}

		bool operator<=(const KeywordToken& kw) const
		{
			return _tcscmp(value, kw.value) <= 0;
		}

		bool listA() { return kwtype_listA(type); }
		bool listB() { return kwtype_listB(type); }
	};

	class BracketToken : public KeywordToken
	{
	public:
		BracketToken(const TCHAR* cvalue, bool isLeft) : KeywordToken(cvalue)
		{
			type = isLeft ? KeywordTokenType::LeftBracket : KeywordTokenType::RightBracket;
		}

		static BracketToken* asBracket(KeywordToken* kw)
		{
			return reinterpret_cast<BracketToken*>(kw);
		}
	};

	class OperatorToken : public KeywordToken
	{
	public:
		Associativity associativity;
		int priority;

		OperatorToken(const TCHAR* cvalue, Arity arity, int priority, Associativity associativity = Associativity::Left)
			: KeywordToken(cvalue), priority(priority), associativity(associativity)
		{
			type = arity == Arity::Unary ? KeywordTokenType::Unary : KeywordTokenType::Binary;
		}

		static OperatorToken* asOperator(KeywordToken* kw)
		{
			return reinterpret_cast<OperatorToken*>(kw);
		}
	};
}