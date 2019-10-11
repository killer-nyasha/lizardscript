#pragma once
#include "interpreter_util.h"
#include "from_sstp/stringptr.h"
#include "LizardScriptCompiler.h"

namespace LizardScript
{
	class LizardScriptLibrary
	{
	public:
		void print(int);
		void print(float);
		void print(stringptr);

		void eval(void* ths, stringptr type, stringptr source);

		static void init_ls_standart_meta();

#define extern
		extern bool printHello = true;
		extern bool printWarnings = true;
		extern bool printDump = true;
		extern bool printState = true;
		extern bool printCompilationTime = true;
		extern bool printRunTime = true;
		extern bool clearScreen = false;
		extern bool printOutput = true;
#undef extern
	};

	extern LizardScriptLibrary global_lsl;
}