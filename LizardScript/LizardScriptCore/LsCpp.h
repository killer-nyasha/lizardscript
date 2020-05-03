#pragma once
#include "LsFunction.h"
#include "StringBuilder.h"

class LsCpp;

struct AbstractLsCppOpcode
{
	LsCode opcode;
	virtual void print(LsCpp& lscpp) = 0;
};

struct LsCppSpec
{
	virtual void append(LsCpp& lscpp) = 0;
};

class LsCpp
{
private:

	std::vector<AbstractLsCppOpcode*> opcodes;

public:

	StringBuilder text;

	void generate(const LsFunction& f);

	struct LsCppSpecR1 : public LsCppSpec
	{
		virtual void append(LsCpp& lscpp) override
		{
			lscpp.text << "1";
		}
	};

	struct LsCppSpecR2 : public LsCppSpec
	{
		virtual void append(LsCpp& lscpp) override
		{
			lscpp.text << "1";
		}
	};

	struct LsCppSpecRegister : public LsCppSpec
	{
		LsCppSpecRegister(LsCppSpec* rnum)
		{

		}

		virtual void append(LsCpp& lscpp) override
		{
			lscpp.text << "r";
		}
	};

	struct LsCppSpecCodeget : public LsCppSpec
	{
		virtual void append(LsCpp& lscpp) override
		{
			lscpp.text << "c";
		}
	};

	LsCppSpec* r1() { return new LsCppSpecR1(); }
	LsCppSpec* r2() { return new LsCppSpecR2(); }

	LsCpp();

	template <typename T>
	LsCppSpec* _register(LsCppSpec* rnum) 
	{
		return new LsCppSpecRegister(rnum);
	}

	template <typename T>
	LsCppSpec* _codeget()
	{
		return new LsCppSpecCodeget();
	}
};