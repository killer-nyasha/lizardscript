#include "stdafx.h"
#include "IByteCodeGenerator.h"
#include "ByteCodeGenerator.h"

namespace LizardScript
{
	//!byte-code compilation
	//!replaces text of operators with their opcodes
	//!\returns ByteCodeExpr object ready to run
	//!\param[in] tokens Sequence of tokens in postfix notation (result of runParser)
	Expr runByteCodeCompiler(TypeInfo type, std::vector<TCHAR*>& tokens)
	{
		return ByteCodeGenerator(tokens, type).e;
	}
}