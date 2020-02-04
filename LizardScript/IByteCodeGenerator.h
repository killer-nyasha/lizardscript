#pragma once
#include "interpreter_util.h"
#include "LsExpr.h"

namespace LizardScript
{
	//class Expr;
	Expr runByteCodeCompiler(TypeInfo type, std::vector<TCHAR*>& tokens);
}