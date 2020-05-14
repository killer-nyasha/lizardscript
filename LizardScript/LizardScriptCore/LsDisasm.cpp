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

std::string LsDisasm::disasm(const LsFunction& _f)
{
	f = &_f;

	while (eip < f->code.size())
	{
		text << "//" << eip << ":\n";

		LsCode code = RUNTIME_CODEGET(LsCode);

		text << opcodeToText[code];

		size_t length = strlen(opcodeToText[code]);

		if (length >= 16)
			throw Exception();

		for (size_t i = 0; i < 16 - length; i++)
			text << " ";

		size_t cgs = lscpp.opcodes_table[code]->codeget_size;

		if (cgs != 0)
		{
			for (size_t i = 0; i < cgs; i++)
			{
				if (i != 0)
					text << ", ";
				text << RUNTIME_CODEGET(unsigned char);
			}
		}

		text << "\n";
	}

	return std::move(text.data);
}