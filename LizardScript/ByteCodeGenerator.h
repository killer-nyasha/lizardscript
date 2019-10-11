#pragma once
#include "interpreter_util.h"
#include "Expr.h"
#include "SyntaxCore.h"
#include "RegisterAllocator.h"

namespace LizardScript
{
	inline bool is_x64()
	{
		return sizeof(void*) == 8;
	}

	class Keyword2
	{
	public:

		static SyntaxCore* core;

		int arity = 2;
		Keyword* text;
		opcode opcodes[4];
		regindex_pair pairs[4];
		int opcodesCount;
		int ptr[2];
		TypeInfo type[2];
		TypeInfo rettype;

		Keyword2(const TCHAR* text, int ind1, int ind2, TypeInfo type1, TypeInfo type2)
		{
			Keyword kw(text);
			this->text = &core->keywords[vectorBinarySearch(core->keywords, kw)];

			ptr[0] = ind1;
			ptr[1] = ind2;

			type[0] = type1;
			type[1] = type2;

			rettype = type1;

			opcodesCount = 0;
		}

		Keyword2& setOpcode(opcode opcode, regindex_pair pair)
		{
			opcodes[opcodesCount] = opcode;
			pairs[opcodesCount] = pair;
			return *this;
		}

		Keyword2& setOpcode(opcode opcode, regindex p1, regindex p2)
		{
			opcodes[opcodesCount] = opcode;
			pairs[opcodesCount] = regindex_pair(p1, p2);
			opcodesCount++;
			return *this;
		}

		Keyword2& setArity(int a)
		{
			arity = a;
			return *this;
		}
	};

	template <typename T1>
	Keyword2 keyword1(const TCHAR* text, int ind1)
	{
		return Keyword2(text, ind1, ind1, makeTypeInfo<T1>(), makeTypeInfo<T1>()).setArity(1);
	}

	template <typename T1, typename T2>
	Keyword2 keyword2(const TCHAR* text, int ind1, int ind2)
	{
		return Keyword2(text, ind1, ind2, makeTypeInfo<T1>(), makeTypeInfo<T2>());
	}

	template <typename R, typename T1, typename T2>
	Keyword2 keyword2(const TCHAR* text, int ind1, int ind2)
	{
		auto kw = Keyword2(text, ind1, ind2, makeTypeInfo<T1>(), makeTypeInfo<T2>());
		kw.rettype = makeTypeInfo<R>();
		return kw;
	}

	struct jmp
	{
		int codeIndex;
		int toTokenIndex;

		int toCodeBackIndex = -1;
	};

	struct futurejmp
	{
		int fromTokenIndex;
		int toTokenIndex;
	};

	struct PossibleFunctionCalls
	{
		std::vector<FunctionInfo*> functions;
		regindex index;
		bool isIf = false, isWhile = false;
		int codeIndex, tokenIndex;
		int ntokenIndex, elseNtokenIndex = 0;
	};

	class ByteCodeGenerator
	{
	public:
		ByteCodeGenerator(std::vector<TCHAR*>& tokens, TypeInfo type, SyntaxCore& core);

		Expr e;

	private:

		ByteCode& code;
		std::vector<TCHAR*>& tokens;
		SyntaxCore& core;
		TypeInfo type;
		RegisterAllocator<16> reg;

		std::vector<jmp>& jmps;
		std::vector<futurejmp>& fjmps;
		std::vector<FieldInfo>& localVar;
		std::stack<PossibleFunctionCalls>& functionCalls;

		int localVarOffset = 0;
		int& localVarMaxOffset;

		const std::vector<Keyword2>& initKeywords();
		void processJumps(int tIndex);
		int findEndLine(std::vector<TCHAR*>::iterator ptoken);
		void open_reg(typed_reg& r, int ptrLevel);
		void identifiersProcessor(std::vector<TCHAR*>::iterator& ptoken);
	};
}