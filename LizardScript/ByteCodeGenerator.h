#pragma once
#include "interpreter_util.h"
#include "LsExpr.h"
#include "RegisterAllocator.h"
#include "Tokens.h"
#include "SemanticCore.h"
#include "Pools.h"

namespace LizardScript
{
	inline bool is_x64()
	{
		return sizeof(void*) == 8;
	}

	inline size_t reg_size()
	{
		return sizeof(void*);
	}

	inline size_t ptr_size()
	{
		return sizeof(void*);
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

	struct LocalVarAddr
	{
		int byteCodeOffset;
		TCHAR* varName;
	};

	class ByteCodeGenerator
	{
	public:
		ByteCodeGenerator(SemanticCore& core, std::vector<TCHAR*>& tokens, TypeInfo type, bool optimized = false);

	private:

		//!input tokens
		std::vector<TCHAR*>& tokens;

		//!type of "this"
		TypeInfo type;

		bool optimized = false;

		//!output expression
		Expr* e = new Expr();

		//!reference to e->code
		ByteCode& code;

		RegisterAllocator<16> reg;

		PoolPointer<NonTypedStack> optStack;

		PoolPointer<std::vector<jmp>> jmps;
		PoolPointer<std::vector<futurejmp>> fjmps;
		PoolPointer<std::vector<FieldInfo>> localVar;
		PoolPointer<std::stack<PossibleFunctionCalls>> functionCalls;
		PoolPointer<std::vector<LocalVarAddr>> localVarAddr;

		PoolPointer<std::stack<size_t>> localVarLevels;
		PoolPointer<std::stack<size_t>> functionCallsLevels;

		FieldInfo& newLocalVariable(TCHAR* type, const TCHAR* name);
		FieldInfo& newLocalVariable(TypeInfo info, const TCHAR* name, bool getAddressImmediately = true);

		int localVarOffset = 0;
		int& localVarMaxOffset;

		SemanticCore& core;

		void processJumps(int tIndex);
		int findEndLine(std::vector<TCHAR*>::iterator ptoken);
		bool/*?*/ open_reg(typed_reg& r, int ptrLevel);
		void identifiersProcessor(std::vector<TCHAR*>::iterator& ptoken);

		bool addUnary(KeywordToken* kwtoken, typed_reg r1);
		bool addBinary(KeywordToken* kwtoken, typed_reg r1, typed_reg r2);
		bool cast(typed_reg reg, TypeInfo to);

		bool experimental_paramsForwarding = false;

		void findFunctionToCall(PossibleFunctionCalls& call);

		void parseLiteral(TCHAR* token);

		template <typename T>
		void push_static(T& value)
		{
			optStack->push(value);
		}

		template <typename T>
		void reg_alloc()
		{
			TypeInfo info = TYPEINFO(T);

			if (info.size() <= reg_size())
			{
				typed_reg& rn = reg.alloc(TYPEINFO(T));
				code << LsOpcode::push_32 << rn;
			}
			else
			{
				//??????????
			}

		}

		template <typename T>
		bool try_push_static(T& value)
		{
			if (optStack->check(value))
			{
				push_static(value);
				return true;
			}
			else 
			{
				reg_alloc(value);
				return false;
			}
		}

	};
}