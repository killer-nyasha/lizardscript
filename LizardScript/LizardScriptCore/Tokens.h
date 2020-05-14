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

	//!now parser doesn't support non-associative operators
	enum class Associativity
	{
		Left,
		Right
	};

	//!type of token
	enum class KeywordTokenType: int
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
		//Override
	};

	enum class CompilerFlags
	{
		None,
		String,
		Call,
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
		CompilerFlags compilerFlags = CompilerFlags::None;

		void* customData = nullptr;

		KeywordToken(const TCHAR* cvalue)
		{
			static_assert(offsetof(KeywordToken, signature) == 0, "pizdec");
			_tcsncpy(value, cvalue, sizeof(value)/sizeof(TCHAR));
		}

		KeywordToken(const TCHAR* cvalue, KeywordTokenType type, CompilerFlags compilerFlags = CompilerFlags::None) : KeywordToken(cvalue)
		{
			this->type = type;
			this->compilerFlags = compilerFlags;
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

		bool is_operator()
		{
			return type == KeywordTokenType::PrefixUnary || type == KeywordTokenType::PostfixUnary ||
				type == KeywordTokenType::Binary;
		}
	};

	//!Token of operator. Any keyword which has KeywordTokenType::Unary or KeywordTokenType::Binary must be derived from this class.
	class OperatorToken : public KeywordToken
	{
	public:
		Associativity associativity;
		int priority;

		OperatorToken(const TCHAR* cvalue, KeywordTokenType type, int priority, Associativity associativity = Associativity::Left, CompilerFlags compilerFlags = CompilerFlags::None)
			: KeywordToken(cvalue, type, compilerFlags), priority(priority), associativity(associativity)
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
