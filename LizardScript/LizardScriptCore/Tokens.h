/*!
\file Tokens.h
\brief Classes of tokens for lexical analyzer
\author killer-nyasha
\version 0.2
\date 17.01.2020
*/
#pragma once
#include "crossplatform_tchar.h"

namespace LizardScript
{
	//!some special algorithms when parser should do when it meet an operator with this flags
	enum class ParserFlags
	{
		None = 0,
		Comma,
		EndLine
	};

	////!unary or binary. ternary operators can be realized via binary
	//enum class Arity
	//{
	//	Unary,
	//	Binary
	//};

	//!now parser doesn't support non-associative operators
	enum class Associativity
	{
		Left,
		Right
	};

	//!type of token
	enum class KeywordTokenType
	{
		//!returns some value. for example, 'this'
		Simple,

		//!prefix unary operator
		PrefixUnary,

		//!postfix unary operator
		PostfixUnary,

		//!binary operator
		Binary,

		//!left bracket, like (, [ or {
		LeftBracket,

		//!right bracket, like ), ] or }
		RightBracket,

		//!some custom actions
		Override
	};

	inline TCHAR kwtypes_str(KeywordTokenType t)
	{
		switch (t)
		{
		case KeywordTokenType::Simple:
			return 's';
		case KeywordTokenType::PrefixUnary:
			return 'u';
		case KeywordTokenType::PostfixUnary:
			return 'U';
		case KeywordTokenType::Binary:
			return 'b';
		case KeywordTokenType::LeftBracket:
			return '(';
		case KeywordTokenType::RightBracket:
			return ')';
		default:
			return '?';
		}
	}

	//!"list A" - token types which we can meet right after a binary operator.
	//!"list B" - token types which we can meet right after some value.
	//enum class LexerList
	//{
	//	A,
	//	B,
	//};
	
	////!"list A" - token types which we can meet right after a binary operator.
	////!\returns is this type a part of "list A"
	//inline bool kwtype_listA(KeywordTokenType type)
	//{
	//	return type == KeywordTokenType::Simple
	//		|| type == KeywordTokenType::Unary
	//		|| type == KeywordTokenType::LeftBracket;
	//}

	////!"list B" - token types which we can meet right after a unary operator (or simple value??).
	////!\returns is this type a part of "list B"
	//inline bool kwtype_listB(KeywordTokenType type)
	//{
	//	return type == KeywordTokenType::Binary
	//		|| type == KeywordTokenType::LeftBracket
	//		|| type == KeywordTokenType::RightBracket;
	//}

	////!\returns is after this type can be a type from "list A"
	//inline bool kwtype_before_listA(KeywordTokenType type)
	//{
	//	return type == KeywordTokenType::Unary
	//		|| type == KeywordTokenType::Binary
	//		|| type == KeywordTokenType::LeftBracket;
	//}

	////!\returns is after this type can be a type from "list B"
	//inline bool kwtype_before_listB(KeywordTokenType type)
	//{
	//	return type == KeywordTokenType::Simple
	//		|| type == KeywordTokenType::Unary
	//		|| type == KeywordTokenType::RightBracket;
	//}

	//!Base class for any token
	//\warning children can be converted to KeywordToken by reinterpret_cast
	//!\warning field signature must be first (offset 0). you mustn't change the signature, otherwise parser won't interpret object as a KeywordToken
	class KeywordToken
	{
#define IKEYWORD_SIGNATURE ('~'*256*256*256 + 'k'*256*256 + 'w'*256 + '~')
		const int signature = IKEYWORD_SIGNATURE;
		
	public:

		//!text representation of a keyword
		TCHAR value[16];

		KeywordTokenType type = KeywordTokenType::Simple;
		ParserFlags parserFlags = ParserFlags::None;

		KeywordToken(const TCHAR* cvalue)
		{
			static_assert(offsetof(KeywordToken, signature) == 0, "pizdec");
			_tcsncpy(value, cvalue, sizeof(value)/sizeof(TCHAR));
		}

		KeywordToken(const TCHAR* cvalue, KeywordTokenType type) : KeywordToken(cvalue)
		{
			this->type = type;
		}

		//!\warning may be dangerous
		static inline bool isKeyword(const void* token)
		{
			return reinterpret_cast<const KeywordToken*>(token)->signature == IKEYWORD_SIGNATURE;
		}

		//!compare text representation of keyword with a string
		bool operator==(const TCHAR* kw) const
		{
			return _tcscmp(value, kw) == 0;
		}

		//!compare text representation of keyword with a string
		bool operator<(const TCHAR* kw) const
		{
			return _tcscmp(value, kw) < 0;
		}

		//!compare text representation of keyword with a string
		bool operator>=(const TCHAR* kw) const
		{
			return _tcscmp(value, kw) >= 0;
		}

		//!compare text representation of keyword with a string
		bool operator<=(const TCHAR* kw) const
		{
			return _tcscmp(value, kw) <= 0;
		}

		//!compare text representation of keywords
		bool operator==(const KeywordToken &kw) const 
		{ 
			return _tcscmp(value, kw.value) == 0; 
		}

		//!compare text representation of keywords
		bool operator<(const KeywordToken& kw) const
		{
			return _tcscmp(value, kw.value) < 0;
		}

		//!compare text representation of keywords
		bool operator>=(const KeywordToken& kw) const
		{
			return _tcscmp(value, kw.value) >= 0;
		}

		//!compare text representation of keywords
		bool operator<=(const KeywordToken& kw) const
		{
			return _tcscmp(value, kw.value) <= 0;
		}

		template <typename T>
		friend T& operator<<(T& stream, const KeywordToken & t)
		{
			stream << t.value;
			return stream;
		}

		//bool listA() { return kwtype_listA(type); }
		//bool listB() { return kwtype_listB(type); }

		bool is_operator()
		{
			return type == KeywordTokenType::PrefixUnary || type == KeywordTokenType::PostfixUnary ||
				type == KeywordTokenType::Binary;
		}
	};

	//!Token of bracket. Any keyword which has KeywordTokenType::LeftBracket or KeywordTokenType::RightBracket must be derived from this class.
	//class BracketToken : public KeywordToken
	//{
	//public:
	//	BracketToken(const TCHAR* cvalue, bool isLeft) : KeywordToken(cvalue)
	//	{
	//		type = isLeft ? KeywordTokenType::LeftBracket : KeywordTokenType::RightBracket;
	//	}

	//	static BracketToken* asBracket(KeywordToken* kw)
	//	{
	//		return reinterpret_cast<BracketToken*>(kw);
	//	}

	//	static const BracketToken* asBracket(const KeywordToken* kw)
	//	{
	//		return reinterpret_cast<const BracketToken*>(kw);
	//	}
	//};

	//!Token of operator. Any keyword which has KeywordTokenType::Unary or KeywordTokenType::Binary must be derived from this class.
	class OperatorToken : public KeywordToken
	{
	public:
		Associativity associativity;
		int priority;

		OperatorToken(const TCHAR* cvalue, KeywordTokenType type, int priority, Associativity associativity = Associativity::Left)
			: KeywordToken(cvalue), priority(priority), associativity(associativity)
		{
			this->type = type;// arity == Arity::Unary ? KeywordTokenType::Unary : KeywordTokenType::Binary;
		}

		static OperatorToken* asOperator(KeywordToken* kw)
		{
			return reinterpret_cast<OperatorToken*>(kw);
		}

		static const OperatorToken* asOperator(const KeywordToken* kw)
		{
			return reinterpret_cast<const OperatorToken*>(kw);
		}
	};
}