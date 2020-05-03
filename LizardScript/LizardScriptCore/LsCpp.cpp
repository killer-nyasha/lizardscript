#include "pch.h"
#include <tuple>
#include <iostream>

#include "LsCpp.h"
#include "Opcodes.hxx"

#define REGISTER(T, i) new LsCppSpecRegister(#T, i)
#define CODEGET(T) new LsCppSpecCodeget(#T, sizeof(T))

void append(LsCpp& lscpp, const char* data)
{
	lscpp.text << data;
}

void append(LsCpp& lscpp, LsCppSpec* spec)
{
	spec->append(lscpp);
}

template <typename... T>
struct LsCppOpcodeT : public AbstractLsCppOpcode
{
	std::tuple<T...> data;

	LsCppOpcodeT(LsCode opcode, T... args)
	{
		this->opcode = opcode;
		data = std::make_tuple(args...);
	}

	template <size_t... S>
	void _vforeach(LsCpp& lscpp, std::index_sequence<S...> s)
	{
		char c[] = { (append(lscpp, std::get<S>(data)), '0')... };
	}

	void vforeach(LsCpp& lscpp)
	{
		_vforeach(lscpp, std::make_index_sequence<sizeof...(T)>());
	}

	virtual void print(LsCpp& lscpp) override
	{
		vforeach(lscpp);
	}
};

template <typename... T>
LsCppOpcodeT<T...>* LsCppOpcode(LsCode opcode, T... args)
{
	return new LsCppOpcodeT<T...>(opcode, args...);
}

LsCpp::LsCpp()
{
	opcodes =
	{
		LsCppOpcode(LsAsm::add_int, "{ *",REGISTER(int, this->r1())," = *",REGISTER(int, this->r1())," + *",REGISTER(int, this->r2()),"; break; }")
		//#include "LsCpp.hxx"
	};

	for (auto* op : opcodes)
	{
		opcodes_table[op->opcode] = op;
	}
}

#define RUNTIME_REGISTER(type, i) reinterpret_cast<type*>(&stack[esp + i])
#define RUNTIME_CODEGET(type) *(type*)(&f->code[(eip += sizeof(type)) - sizeof(type)])
#define LSCPP_RUNTIME_CODEGET(type) *(type*)(&lscpp.f->code[(lscpp.eip += sizeof(type)) - sizeof(type)])

void LsCpp::LsCppSpecCodeget::append(LsCpp& lscpp)
{
	lscpp.text << "FROM_BINARY(";

	for (size_t i = 0; i < size; i++)
	{
		if (i != 0)
			lscpp.text << ", ";
		lscpp.text << LSCPP_RUNTIME_CODEGET(unsigned char);
	}

	lscpp.text << ")";
}

void LsCpp::generate(const LsFunction& _f)
{
	f = &_f;

	while (eip < f->code.size())
	{
		LsCode code = RUNTIME_CODEGET(LsCode);
		_r1 = RUNTIME_CODEGET(OFFSET_T);
		_r2 = RUNTIME_CODEGET(OFFSET_T);

		opcodes_table[code]->print(*this);
		text << "\n";
	}

	std::cout << text.data;

}