#include "pch.h"
#include <tuple>
#include <iostream>

#include "LsCpp.h"
#include "Opcodes.hxx"

#define REGISTER(T, i) _register<T>(i)
#define CODEGET(T) _codeget<T>()

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
		//FORVARIADIC(std::cout << data);
	}

	//template <size_t i>
	//void _append()
	//{
	//	append(lscpp, std::get<i>(data));
	//}

	//template <size_t i>
	//void append(std::integer_sequence<i> seq, LsCpp& lscpp)
	//{
	//	FORVARIADIC(_append<i>());
	//}
	//

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
		LsCppOpcode(LsAsm::add_int, "{ *",this->REGISTER(int, this->r1())," = *",REGISTER(int, this->r1())," + *",REGISTER(int, this->r2()),"; break; }")
		//#include "LsCpp.hxx"
	};
}

void LsCpp::generate(const LsFunction& f)
{
	for (auto* op : opcodes)
	{
		op->print(*this);
	}

	std::cout << text.data;
}