#pragma once
#include "SyntaxCore.h"
#include "LsExpr.h"

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
	};

}