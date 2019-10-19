#include "opcodes_template.h"

#define DO(body) { body; break; }

case opcode::set_32: DO(opcode_set(int, int))//int = int
case opcode::set_64: DO(opcode_set(long long int, long long int))//int = int

case opcode::set_big: 
{
	memcpy(OPEN(void*, rnfirst), OPEN(void*, rnsecond), CODEGET(short));
	break;
}

//case opcode::set_float_float: DO(opcode_set(float, float))//float = float

case opcode::push_stringptr:
{
	int allocSize = CODEGET(int);

	std::string str = std::string(allocSize, ' ');

	for (size_t j = 0; j < allocSize; j++)
	{
		str[j] = CODEGET(char);
	}

	stringptr p = stringptr(std::move(str));
	registers[rnfirst] = *reinterpret_cast<void**>(&p);
	p.pointer = nullptr;

	//i += allocSize;
	//i -= 2;
	break;
}

case opcode::set_stringptr:
{
	opcode_set(stringptr, stringptr);

	//*reinterpret_cast<stringptr*>(registers[rnfirst])
	//	= *reinterpret_cast<stringptr*>(&registers[rnsecond]);
	break;
}

case opcode::alloc:
{
	int allocSize = CODEGET(short);
	registers[rnfirst] = (void*)(new char[allocSize]);
	break;
}

case opcode::more_int_int:
{
	OPEN(int, rnfirst) = 
		OPEN(int, rnfirst) > OPEN(int, rnsecond) ? 1 : 0;
	break;
}
case opcode::less_int_int:
{
	OPEN(int, rnfirst) =
		OPEN(int, rnfirst) < OPEN(int, rnsecond) ? 1 : 0;
	break;
}

case opcode::jz://учитывает все 8 байт
{
	int toIndex = CODEGET(int);
	if (OPEN(int, rnfirst) == 0)
	{
		i = toIndex - 2;
	}
	break;
}
case opcode::jmp:
{
	int toIndex = CODEGET(int);
	i = toIndex - 2;
	break;
}

case opcode::call_cpp:
{
	DummyCallStruct* callStruct = reinterpret_cast<DummyCallStruct*>(&expr.code.data[i]);
	callStruct->call(registers, (int)rnfirst);
	i += sizeof(FunctionInfo().callStruct);
	break;
}

case opcode::push_32: DO(opcode_push_const(int)) //push int const
case opcode::push_64: DO(opcode_push_const(long long int)) //push int const
case opcode::push_this://push this
{
	registers[rnfirst] = ths; 
	break;
}
case opcode::push_stackptr://push this
{
	registers[rnfirst] = stackdata;
	break;
}
case opcode::push_lsl://push this
{
	registers[rnfirst] = (void*)&lsl;
	break;
}
case opcode::push_offset://push offset
{
	registers[rnsecond] = (void*)(OPEN(char*, rnfirst) + CODEGET(short));
	break;
}
case opcode::set_this://goto ptr
{
	registers[rnfirst] = *(void**)(registers[rnsecond]);	
	break;
}
case opcode::mov://mov
{
	registers[rnfirst] = registers[rnsecond];
	break;
}
case opcode::get_32://get int
{
	int f = *(int*)registers[rnsecond];
	registers[rnfirst] = *(void**)&f;	
	break;
}
case opcode::get_64://get float
{
	long long int f = *(long long int*)registers[rnsecond];
	registers[rnfirst] = *(void**)&f;
	break;
}

case opcode::add_int_int://int + int
{
	OPEN(int, rnfirst) += OPEN(int, rnsecond);
	break;
}
case opcode::sub_int_int://int - int
{
	OPEN(int, rnfirst) -= OPEN(int, rnsecond);
	break;
}
case opcode::mul_int_int://int * int
{
	OPEN(int, rnfirst) *= OPEN(int, rnsecond);
	break;
}
case opcode::div_int_int://int / int
{
	OPEN(int, rnfirst) /= OPEN(int, rnsecond);
	break;
}

case opcode::int_to_float://int / int
{
	OPEN(float, rnfirst) = OPEN(int, rnsecond);
	break;
}
case opcode::float_to_int://int / int
{
	OPEN(int, rnfirst) = OPEN(float, rnsecond);
	break;
}

case opcode::add_float_float://float + float
{
	OPEN(float, rnfirst) += OPEN(float, rnsecond);
	break;
}
case opcode::sub_float_float://float - float
{
	OPEN(float, rnfirst) -= OPEN(float, rnsecond);
	break;
}
case opcode::mul_float_float://float * float
{
	OPEN(float, rnfirst) *= OPEN(float, rnsecond);
	break;
}
case opcode::div_float_float://float / float
{
	OPEN(float, rnfirst) /= OPEN(float, rnsecond);
	break;
}

case opcode::inc:
{
	(*OPEN(int*, rnsecond))++;
	break;
}
case opcode::dec:
{
	(*OPEN(int*, rnsecond))--;
	break;
}