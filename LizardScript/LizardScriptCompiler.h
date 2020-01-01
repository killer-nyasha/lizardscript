#pragma once
#include "SyntaxCore.h"
#include "Expr.h"
//#include "ByteCodeExpr.h"

namespace LizardScript
{
	//using namespace Lexer;

	//for unit-tests for private members
	//namespace Tests { class InterpreterUnitTests; }

	//!ByteCodeExpr factory class
	//!Contains all functions for translation source text to tokens and byte code.
	class LizardScriptCompiler
	{
	public:
		//friend class Tests::InterpreterUnitTests;

		//!syntax of expressions language
		SyntaxCore& core;

		//!create with default syntax
		LizardScriptCompiler(SyntaxCore& c, bool optimized = false) : core(c), optimized(optimized){ }

		bool optimized;

		//!compile expression to byte code
		//!\returns expression in byte code, ready to run or save
		//!\param[in] text expression source text
		Expr create(TypeInfo type, const TCHAR* text, bool catchEx = true);

		//void create_excpt(TypeInfo type, const TCHAR* text);
		void create_impl(TypeInfo type, const TCHAR* text);

		template <typename T>
		TypedExpr<T> create(const TCHAR* text) { auto expr = create(makeTypeInfo<T>(), text);  return *reinterpret_cast<TypedExpr<T>*>(&expr); }

	protected:

		Expr expr;

		//!lexical analysis
		//!split text to tokens (such as operators, names and values)
		//!\returns ExprCompiler::LexerResult object, which contains tokens
		//!\param[in] t expression source text
		//void runLexer(const TCHAR* t, std::vector<TCHAR>& values, std::vector<TCHAR*>& tokens);

		//!converts infix sequence of tokens to postfix notation
		//!used simplified shunting-yard algorithm
		//!\returns tokens in postfix notation
		//!\param[in] r result of function runLexer
		std::vector<TCHAR*> runParser(std::vector<TCHAR*>& r);

		//!byte-code compilation
		//!replaces text of operators with their opcodes
		//!\returns ByteCodeExpr object ready to run
		//!\param[in] tokens Sequence of tokens in postfix notation
		Expr runByteCodeCompiler(TypeInfo type, std::vector<TCHAR*>& tokens);
	};

	extern LizardScriptCompiler* standartCompiler;
}

//сделать скрипты с параметрами

using namespace LizardScript;

template <typename T>
inline TypedExpr<T> /*operator ""  _script*/ script(const TCHAR* text)
{
	return LizardScript::standartCompiler->create<T>(text);
}