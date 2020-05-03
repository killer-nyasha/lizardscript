#pragma once
#include "LsCpp.h"

namespace LizardScript
{
	class LsDisasm
	{
		LsCpp& lscpp;

	public:

		//base class?
		const LsFunction* f;
		using OFFSET_T = unsigned char;
		size_t eip = 0;
		OFFSET_T _r1 = 0;
		OFFSET_T _r2 = 0;
		StringBuilder text;

		LsDisasm(LsCpp& lscpp);

		void disasm(const LsFunction& _f);
	};
}
