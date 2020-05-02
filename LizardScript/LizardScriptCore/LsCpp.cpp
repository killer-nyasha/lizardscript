#include "pch.h"
#include <tuple>
#include <iostream>

#include "LsCpp.h"
#include "Opcodes.hxx"

#define REGISTER(T, i) _register<T>(i)
#define CODEGET(T) _codeget<T>()

struct AbstractLsCppOpcode
{
	LsCode opcode;
};

template <typename... T>
struct LsCppOpcodeT : public AbstractLsCppOpcode
{
	std::tuple<T...> data;

	LsCppOpcodeT(LsCode opcode, T... args)
	{
		this->opcode = opcode;
		data = std::make_tuple(args);
		FORVARIADIC(std::cout << data);
	}
};

template <typename... T>
LsCppOpcodeT<T...> LsCppOpcode(LsCode opcode, T... args)
{
	return LsCppOpcodeT<T...>(opcode, args);
}

void LsCpp::generate(const LsFunction& f)
{
	std::vector<AbstractLsCppOpcode> opcodes =
	{
		//LsCppOpcode(LsAsm::add_int, "{ *",this->REGISTER(int, this->r1())," = *",REGISTER(int, this->r1())," + *",REGISTER(int, this->r2()),"; break; }")
#include "LsCpp.hxx"
	};
}