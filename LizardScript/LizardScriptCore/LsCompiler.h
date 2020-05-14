#pragma once
#include "SyntaxCore.h"
#include "LizardScriptDefault.h"
#include "LsFunction.h"
#include "OpcodesText.h"
#include "Pools.h"
#include "TypeInfo.h"

namespace LizardScript
{
	using OperatorsMap = std::map<const char*, const char*, cmp_str>;
	using OpcodesMap = std::map<const char*, LsCode, cmp_str>;

	class Operators
	{
	public:

		//!requires opcodes_initialized
		static OpcodesMap forType(OperatorsMap& st_operators, const char* type1, const char* type2 = nullptr);
	};

	class LsCompiler
	{
		SyntaxCore& core;

		std::map<TypeInfo, OpcodesMap> map_binary;
		std::map<TypeInfo, OpcodesMap> map_prefix;
		std::map<TypeInfo, OpcodesMap> map_postfix;

		void addBinaryMap(OperatorsMap& m, const TCHAR* text1, TypeInfo info1);
		void addPrefixMap(OperatorsMap& m, const TCHAR* text1, TypeInfo info1);
		void addPostfixMap(OperatorsMap& m, const TCHAR* text1, TypeInfo info1);

	public:
		LsCompiler(SyntaxCore& core);

		LsCompiler() : LsCompiler(Default::getSyntaxCore())
		{

		}

		LsFunction compile(const TCHAR* text, size_t length = 0);

	};
}