#pragma once
#include "interpreter_util.h"
#include "stringptr.h"
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

		int random(int minValue, int maxValue)
		{
			return rand() % (maxValue - minValue + 1) + minValue;
		}

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

	extern LizardScriptLibrary lsl;
}