#pragma once
#include "Tokens.h"
#include "LsExpr.h"
#include "NonTypedStack.h"

namespace LizardScript
{
	enum class OperatorType
	{
		//!we check type-safety using fields firstType, secondType, retType
		Standart = 0,

		//!we check type-safety manually in function check(...)
		Override = 1
	};

	class IOperator
	{
	public:
		OperatorType type = OperatorType::Standart;

		KeywordToken* text;

		//!Required only if you doesn't override operator's logic.
		opcode code;

		//!Priority in search of suitable operator. If more than one operator are suitable, 
		//!operator with higher priority will be applied
		int priority;
	};

	class UnaryOperator : public IOperator
	{
	public:
		TypeInfo firstType;
		TypeInfo retType;

		virtual TypeInfo check(TypeInfo first) = 0;

		virtual TypeInfo static_apply(Dynamic first) = 0;
		virtual TypeInfo bytecode_apply(TypeInfo first) = 0;
	};

	class BinaryOperator : public IOperator
	{
	public:
		TypeInfo firstType;
		TypeInfo secondType;
		TypeInfo retType;

		virtual TypeInfo check(TypeInfo first, TypeInfo second) = 0;

		virtual void static_apply(Dynamic first, Dynamic second) = 0;
		virtual TypeInfo bytecode_apply(TypeInfo first, TypeInfo second) = 0;
	};

}