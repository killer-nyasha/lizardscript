#include "pch.h"
#include "LsDisasm.h"
#include "OpcodesText.h"
#include "Exception.h"

#include <iostream>

using namespace LizardScript;

#define RUNTIME_REGISTER(type, i) reinterpret_cast<type*>(&stack[esp + i])
#define RUNTIME_CODEGET(type) *(type*)(&f->code[(eip += sizeof(type)) - sizeof(type)])
#define LSCPP_RUNTIME_CODEGET(type) *(type*)(&lscpp.f->code[(lscpp.eip += sizeof(type)) - sizeof(type)])

LsDisasm::LsDisasm(LsCpp& lscpp) : lscpp(lscpp)
{
	fillOpcodeToText();
}

void LsDisasm::disasm(const LsFunction& _f)
{
	f = &_f;

	while (eip < f->code.size())
	{
		LsCode code = RUNTIME_CODEGET(LsCode);
		_r1 = RUNTIME_CODEGET(OFFSET_T);
		_r2 = RUNTIME_CODEGET(OFFSET_T);

		text << opcodeToText[code];

		size_t length = strlen(opcodeToText[code]);

		if (length >= 16)
			throw Exception();

		for (size_t i = 0; i < 16 - length; i++)
			text << " ";

		text << "[" << _r1 << "], [" << _r2 << "]";

		size_t cgs = lscpp.opcodes_table[code]->codeget_size;
		//eip += cgs;

		if (cgs != 0)
		{
			//text << " ";
			for (size_t i = 0; i < cgs; i++)
				text << ", " << RUNTIME_CODEGET(unsigned char);
		}

		//пока нет вывода результатов CODEGET
		//а ещё ведь могут быть строки

		//lscpp.opcodes_table[code]->print(*this);
		text << "\n";
	}

	std::cout << text.data;
}