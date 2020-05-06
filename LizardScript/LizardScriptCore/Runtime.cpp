#include "pch.h"
#include <iostream>

#include "Runtime.h"
#include "LsTypedefs.h"
#include "RuntimeMacros.h"
#include "Exception.h"

#include "Opcodes.hxx"

using namespace LizardScript;
using namespace LsAsm;

void Runtime::run(const LsFunction& f)
{
	//4 байта - last frame LsFunction
	//4 байта - last frame EIP
	//4 байта - last frame ESP			
	//далее параметры
	//далее регистры
	//далее локальные переменные

	//short frame mode - r1 и r2 по 1 байту, максимум 256 адресуемых байт на кадр стека и объект
	//long frame mode - r1 и r2 по 2 байта, до 65536 байт на кадр или объект

	LsInternalAddr eip = 0;
	size_t esp = 0;

	using OFFSET_T = unsigned char;

#define REGISTER(type, i) reinterpret_cast<type*>(&stack[esp + i]) /*(std::cout << (int)i << ": " << *reinterpret_cast<type*>(&stack[esp + i]) << std::endl, reinterpret_cast<type*>(&stack[esp + i]))*/ 
	
	//r == 0 ? *(type*)((char*)stackbase + CODEGET(short)) : *(type*)&(registers[r])
#define CODEGET(type, name) type& name = *(type*)(&f.code[(eip += sizeof(type)) - sizeof(type)])
#define CODE(name) name

#define JMP(new_eip) eip = new_eip;

	while (true)
	{
		CODEGET(LsCode, code);
		//CODEGET(OFFSET_T, r1);
		//CODEGET(OFFSET_T, r2);

		switch (code)
		{
#include "RuntimeCases.i"

		default:
			throw Exception("Unknown opcode ", code, " at ", eip);
		}
	}

end:;

}
