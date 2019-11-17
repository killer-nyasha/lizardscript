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

	class IOperator
	{
	public:
		static SyntaxCore* core;

		Keyword* text;
		opcode code;

		TypeInfo type1;
		TypeInfo rettype;
	};

	class UnaryOperator : public IOperator
	{
	public:
		UnaryOperator(const TCHAR* text, TypeInfo type1, opcode code)
		{
			Keyword kw(text);
			this->text = &core->keywords[vectorBinarySearch(core->keywords, kw)];
			this->code = code;
			this->type1 = type1;
			rettype = type1;
		}

		UnaryOperator(const TCHAR* text, TypeInfo type1, opcode code, TypeInfo rettype)
			: UnaryOperator(text, type1, code)
		{
			this->rettype = rettype;
		}
	};

	class BinaryOperator : public UnaryOperator
	{
	public:
		TypeInfo type2;

		BinaryOperator(const TCHAR* text, TypeInfo type1, TypeInfo type2, opcode code)
			: UnaryOperator(text, type1, code)
		{
			this->type2 = type2;
		}

		BinaryOperator(const TCHAR* text, TypeInfo type1, TypeInfo type2, opcode code, TypeInfo rettype)
			: UnaryOperator(text, type1, code, rettype)
		{
			this->type2 = type2;
		}
	};

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

	struct LocalVarAddr
	{
		int byteCodeOffset;
		TCHAR* varName;
	};

	class ByteCodeGenerator
	{
	public:
		ByteCodeGenerator(std::vector<TCHAR*>& tokens, TypeInfo type, SyntaxCore& core, bool optimized = false);

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
		std::vector<LocalVarAddr>& localVarAddr;

		FieldInfo& newLocalVariable(TCHAR* type, const TCHAR* name);
		FieldInfo& newLocalVariable(TypeInfo info, const TCHAR* name, bool getAddressImmediately = true);

		int localVarOffset = 0;
		int& localVarMaxOffset;

		const std::vector<UnaryOperator>& initUnary(SyntaxCore& core);
		const std::vector<BinaryOperator>& initBinary(SyntaxCore& core);

		void processJumps(int tIndex);
		int findEndLine(std::vector<TCHAR*>::iterator ptoken);
		bool/*?*/ open_reg(typed_reg& r, int ptrLevel);
		void identifiersProcessor(std::vector<TCHAR*>::iterator& ptoken);

		bool addUnary(Keyword* kwtoken, typed_reg r1);
		bool addBinary(Keyword* kwtoken, typed_reg r1, typed_reg r2);
		bool cast(typed_reg reg, TypeInfo to);

		bool experimental_paramsForwarding = false;

		void findFunctionToCall(PossibleFunctionCalls& call);

		bool optimized = false;
	};
}