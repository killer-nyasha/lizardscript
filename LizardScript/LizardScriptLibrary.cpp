#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "LizardScriptLibrary.h"
#include "interpreter_util.h"
#include "FindType.h"
#include "Runtime.h"

using namespace LizardScript;

//#include "../Sstp/stringptr.h"

void LizardScriptLibrary::init_ls_standart_meta()
{
	METAGEN(int);
	METAGEN(float);
	METAGEN(LizardScriptLibrary,
		PARAMS(int)::FUNC(LizardScriptLibrary) WITHNAME(print),
		PARAMS(float)::FUNC(LizardScriptLibrary) WITHNAME(print),
		PARAMS(stringptr)::FUNC(LizardScriptLibrary) WITHNAME(print),
		PARAMS(void*, stringptr, stringptr)::FUNC(LizardScriptLibrary) WITHNAME(eval),
		PARAMS(int, int)::FUNC(LizardScriptLibrary) WITHNAME(random)
	);
	METAGEN(stringptr, /*FUNC(stringptr, operator[]),*/ PARAMS()::FUNC(CtorProvider<stringptr>) WITHNAME(ctor));

}

void LizardScriptLibrary::print(int i)
{
	if (printOutput)
		std::cout << COLOR_GREEN << "OUTPUT: " << i << COLOR_NC << std::endl;
}

void LizardScriptLibrary::print(float f)
{
	if (printOutput)
		std::cout << COLOR_GREEN << "OUTPUT: " << f << COLOR_NC << std::endl;
}

void LizardScriptLibrary::print(stringptr s)
{
	if (printOutput)
		std::cout << COLOR_GREEN << "OUTPUT: " << s << COLOR_NC << std::endl;
}

void LizardScriptLibrary::eval(void* ths, stringptr type, stringptr source)
{
	if (printOutput)
	{
		print("EVAL"_sp);
		print(type);
		print(source);
	}

	TypeInfo typeInfo = findType(&type->operator[](0));
	Expr e = standartCompiler->create(typeInfo, &source->operator[](0));
	Runtime r(e, typeInfo, ths);
}

namespace LizardScript
{
	LizardScriptLibrary lsl; 
}