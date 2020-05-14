#include "pch.h"
#include <tuple>
#include <iostream>
#include <type_traits>
#include <utility>

#include "LsCpp.h"
#include "Opcodes.hxx"

//cmp_str
#include "OpcodesText.h"

#include "LsTypedefs.h"

#define REGISTER(T, i) LsCppSpecRegister(#T, i)

#define CODEGETSTR(N) LsCppSpecCodegetStr("const char*", #N)
#define CODEGET(T, N) LsCppSpecCodeget(#T, sizeof(T), #N)
#define CODE(N) LsCppSpecCode(#N)

#define JMP(a) LsCppSpecJmp(a)

#define RUNTIME_REGISTER(type, i) reinterpret_cast<type*>(&stack[esp + i])
#define RUNTIME_CODEGET(type) *(type*)(&f->code[(eip += sizeof(type)) - sizeof(type)])
#define LSCPP_RUNTIME_CODEGET(type) *(type*)(&lscpp.f->code[(lscpp.eip += sizeof(type)) - sizeof(type)])
#define LSCPP_RUNTIME_CODEGETSTR(name) char* name = (char*)(&lscpp.f->code[lscpp.eip]); lscpp.eip += strlen(name);

struct LsCppSpecCodeget : public LsCppSpec
{
	const char* type;
	size_t size;
	const char* name;

	LsCppSpecCodeget() { }

	LsCppSpecCodeget(const char* type, size_t size, const char* name) : type(type), size(size), name(name)
	{

	}

	void append(LsCpp& lscpp)
	{
		auto& variables = lscpp.variables;

		if (variables.find(name) == variables.end())
			variables.insert(std::make_pair(name, LsCppVariable()));

		auto& var = variables[name];

		var.type = type;
		var.data_size = 0;// .clear();

		for (size_t i = 0; i < size; i++)
			var.data[var.data_size++] = (LSCPP_RUNTIME_CODEGET(unsigned char));

		lscpp.text << type << " " << name << " = FROM_BINARY<" << var.type << ">::get(";
		for (size_t i = 0; i < var.data_size; i++)
		{
			if (i != 0)
				lscpp.text << ", ";
			lscpp.text << var.data[i];
		}
		lscpp.text << ")";
	}
};

struct LsCppSpecCodegetStr : public LsCppSpec
{
	const char* type;
	const char* name;

	LsCppSpecCodegetStr() { }

	LsCppSpecCodegetStr(const char* type, const char* name) : type(type), name(name)
	{

	}

	void append(LsCpp& lscpp)
	{
		auto& variables = lscpp.variables;

		if (variables.find(name) == variables.end())
			variables.insert(std::make_pair(name, LsCppVariable()));

		auto& var = variables[name];

		var.type = type;
		var.data_size = 0;// .clear();

		LSCPP_RUNTIME_CODEGETSTR(text);

		//for (size_t i = 0; i < size; i++)
		//	var.data[var.data_size++] = (LSCPP_RUNTIME_CODEGET(unsigned char));

		lscpp.text << type << " " << name << " = \"";
		//for (size_t i = 0; i < var.data_size; i++)
		//{
			//if (i != 0)
			//	lscpp.text << ", ";
			lscpp.text << text;
		//}
		lscpp.text << "\"";
	}
};


struct LsCppSpecCode : public LsCppSpec
{
	const char* name;

	LsCppSpecCode() { }

	LsCppSpecCode(const char* name) : name(name)
	{

	}

	void append(LsCpp& lscpp)
	{
		//auto& var = variables[name];

		lscpp.text << "CODE(" << name << ")";

	}
};

struct LsCppSpecRegister : public LsCppSpec
{
	LsCppSpecCode rnum;
	const char* type;

	LsCppSpecRegister() { }

	LsCppSpecRegister(const char* type, LsCppSpecCode rnum) : type(type), rnum(rnum)
	{

	}

	void append(LsCpp& lscpp)
	{
		lscpp.text << "REGISTER(" << type << ", ";
		rnum.append(lscpp);
		lscpp.text << ")";
	}
};

struct LsCppSpecJmp : public LsCppSpec
{
	//size_t addr;
	LsCppSpecCode code;

	//LsCppSpecJmp(size_t addr) : addr(addr)
	//{

	//}

	LsCppSpecJmp() { }

	LsCppSpecJmp(LsCppSpecCode code) : code(code)
	{
		
	}

	void append(LsCpp& lscpp)
	{
		//не уверен в этом моменте
		lscpp.text << "goto lsaddr_" << *(LsInternalAddr*)&lscpp.variables[code.name].data[0] << ";";
	}
};

void append(LsCpp& lscpp, const char* data)
{
	lscpp.text << data;
}

template <typename TS>
void append(LsCpp& lscpp, TS& spec)
{
	spec.append(lscpp);
}


void _inc_codeget_size(AbstractLsCppOpcode* opcode, const char* data)
{
	
}

template <typename TS>
void _inc_codeget_size(AbstractLsCppOpcode* opcode, TS& spec)
{
	if (dynamic_cast<LsCppSpecCodeget*>(&spec))
		opcode->codeget_size += dynamic_cast<LsCppSpecCodeget*>(&spec)->size;
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

	virtual void print(LsCpp& lscpp) override
	{
		_vforeach(lscpp, std::make_index_sequence<sizeof...(T)>());
	}

	template <size_t... S>
	void _vforeach2(std::index_sequence<S...> s)
	{
		char c[] = { (_inc_codeget_size(this, std::get<S>(data)), '0')... };
	}

	virtual void inc_codeget_size() override
	{
		_vforeach2(std::make_index_sequence<sizeof...(T)>());
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
		//LsCppOpcode(LsAsm::add_int, "{ *",REGISTER(int, this->r1())," = *",REGISTER(int, this->r1())," + *",REGISTER(int, this->r2()),"; break; }")
		#include "LsCpp.hxx"
	};

	for (auto* op : opcodes)
	{
		opcodes_table[op->opcode] = op;
		op->inc_codeget_size();
	}
}

std::string LsCpp::generate(const LsFunction& _f)
{
	text << "#include <iostream>\n#include <pch.h>\n#include <LsCppExternal.h>\n#pragma comment(lib, \"LizardScriptCore.lib\")\n";
	text << "int main() {\n";
	text << "unsigned char stack[1024 * 10];\n";
	text << "LsInternalAddr eip = 0;\n";
	text << "size_t esp = 0;\n";


	f = &_f;

	while (eip < f->code.size())
	{
		text << "lsaddr_" << eip << ":;\n";

		LsCode code = RUNTIME_CODEGET(LsCode);

		opcodes_table[code]->print(*this);
		text << "\n";

		//std::cout << text.data;
		//text.data.clear();
	}

	text << "end:; return 0;\n}";

	//std::cout << "end0\n";

	//std::cout << text.data;
	return text.data;
}
