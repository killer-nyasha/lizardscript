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
		PARAMS(int)::FUNC(LizardScriptLibrary, print),
		PARAMS(float)::FUNC(LizardScriptLibrary, print),
		PARAMS(stringptr)::FUNC(LizardScriptLibrary, print),
		FUNC(LizardScriptLibrary, eval),
		PARAMS(int, int)::FUNC(LizardScriptLibrary, random)
	);
	METAGEN(stringptr, FUNC(stringptr, operator[]), FUNC(CtorProvider<stringptr>, ctor));

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