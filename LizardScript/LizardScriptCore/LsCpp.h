#pragma once
#include <map>

#include "OpcodesText.h"
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
	//virtual void append(LsCpp& lscpp) = 0;
};

struct LsCppVariable
{
	const char* type;
	//const char* name;
	//std::vector<unsigned char> data;

	unsigned char data[32];
	size_t data_size = 0;
};

class LsCpp
{
private:

	std::vector<AbstractLsCppOpcode*> opcodes;

public:

	std::map<const char*, LsCppVariable, cmp_str> variables;

	AbstractLsCppOpcode* opcodes_table[256];
	
	//base class?
	const LsFunction* f;
	using OFFSET_T = unsigned char;
	size_t eip = 0;
	StringBuilder text;

	void generate(const LsFunction& f);

	LsCpp();
};