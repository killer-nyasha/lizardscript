#pragma once
#include "LsFunction.h"

struct LsCppSpec
{

};

class LsCpp
{
public:
	void generate(const LsFunction& f);

	LsCppSpec r1() { return LsCppSpec(); }
	LsCppSpec r2() { return LsCppSpec(); }

	template <typename T>
	LsCppSpec _register(LsCppSpec rnum) 
	{
		return LsCppSpec();
	}

	template <typename T>
	LsCppSpec _codeget()
	{
		return LsCppSpec();
	}
};