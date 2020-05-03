#include "pch.h"
#include "LsDisasm.h"
#include "OpcodesText.h"

#include <iostream>

#define RUNTIME_REGISTER(type, i) reinterpret_cast<type*>(&stack[esp + i])
#define RUNTIME_CODEGET(type) *(type*)(&f->code[(eip += sizeof(type)) - sizeof(type)])
#define LSCPP_RUNTIME_CODEGET(type) *(type*)(&lscpp.f->code[(lscpp.eip += sizeof(type)) - sizeof(type)])

void LsDisasm::disasm(const LsFunction& _f)
{
	f = &_f;

	while (eip < f->code.size())
	{
		LsCode code = RUNTIME_CODEGET(LsCode);
		_r1 = RUNTIME_CODEGET(OFFSET_T);
		_r2 = RUNTIME_CODEGET(OFFSET_T);

		//lscpp.opcodes_table[code]->print(*this);
		text << "\n";
	}

	std::cout << text.data;
}