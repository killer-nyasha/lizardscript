#pragma once
#include "LsFunction.h"
#include "StringBuilder.h"

class LsCpp;

struct AbstractLsCppOpcode
{
	LsCode opcode;
	virtual void print(LsCpp& lscpp) = 0;
	size_t codeget_size = 0;	
	virtual void inc_codeget_size() = 0;
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

	AbstractLsCppOpcode* opcodes_table[256];
	
	//base class?
	const LsFunction* f;
	using OFFSET_T = unsigned char;
	size_t eip = 0;
	OFFSET_T _r1 = 0;
	OFFSET_T _r2 = 0;
	StringBuilder text;

	void generate(const LsFunction& f);

	struct LsCppSpecR1 : public LsCppSpec
	{
		virtual void append(LsCpp& lscpp) override
		{
			lscpp.text << lscpp._r1;
		}
	};

	struct LsCppSpecR2 : public LsCppSpec
	{
		virtual void append(LsCpp& lscpp) override
		{
			lscpp.text << lscpp._r2;
		}
	};

	struct LsCppSpecRegister : public LsCppSpec
	{
		LsCppSpec* rnum;
		const char* type;

		LsCppSpecRegister(const char* type, LsCppSpec* rnum) : type(type), rnum(rnum)
		{

		}

		virtual void append(LsCpp& lscpp) override
		{
			lscpp.text << "REGISTER(" << type << ", ";
			rnum->append(lscpp); 
			lscpp.text << ")";
		}
	};

	struct LsCppSpecCodeget : public LsCppSpec
	{
		const char* type;
		size_t size;

		LsCppSpecCodeget(const char* type, size_t size) : type(type), size(size)
		{

		}

		virtual void append(LsCpp& lscpp) override;
	};

	LsCppSpec* r1() { return new LsCppSpecR1(); }
	LsCppSpec* r2() { return new LsCppSpecR2(); }

	LsCpp();
};