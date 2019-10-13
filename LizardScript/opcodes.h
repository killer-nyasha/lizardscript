#include "opcodes_template.h"

#define DO(body) { body; break; }

case opcode::set_32: DO(opcode_set(int, int))//int = int
case opcode::set_64: DO(opcode_set(long long int, long long int))//int = int

//case opcode::set_float_float: DO(opcode_set(float, float))//float = float

case opcode::push_stringptr:
{
	i += 2;
	int allocSize = *reinterpret_cast<int*>(&expr.code.data[i]);
	i += sizeof(int);

	std::string str = std::string(allocSize, ' ');

	for (size_t j = 0; j < allocSize; j++)
	{
		str[j] = *reinterpret_cast<char*>(&expr.code.data[i + j]);
	}

	stringptr p = stringptr(std::move(str));
	registers[rnfirst] = *reinterpret_cast<void**>(&p);
	p.pointer = nullptr;

	i += allocSize;
	i -= 2;
	break;
}

case opcode::set_stringptr:
{
	*reinterpret_cast<stringptr*>(registers[rnfirst])
		= *reinterpret_cast<stringptr*>(&registers[rnsecond]);
	break;
}

case opcode::alloc:
{
	int allocSize = *reinterpret_cast<short int*>(&expr.code.data[i + 2]);
	registers[rnfirst] = (void*)(new char[allocSize]);
	i += sizeof(short int);
	break;
}

case opcode::more_int_int:
{
	*(int*)&registers[rnfirst] = 
		*(int*)&registers[rnfirst] > *(int*)&registers[rnsecond] ? 1 : 0;
	break;
}
case opcode::less_int_int:
{
	*(int*)&registers[rnfirst] =
		*(int*)&registers[rnfirst] < *(int*)&registers[rnsecond] ? 1 : 0;
	break;
}

case opcode::jz://учитывает все 8 байт
{
	if ((int)registers[rnfirst] == 0)
	{
		int toIndex = *reinterpret_cast<int*>(&expr.code.data[i + 2]);
		i = toIndex - 2;
	}
	else i += sizeof(int);
	break;
}
case opcode::jmp:
{
	int toIndex = *reinterpret_cast<int*>(&expr.code.data[i + 2]);
	i = toIndex - 2;
	break;
}

case opcode::call_cpp:
{
	DummyCallStruct* callStruct = reinterpret_cast<DummyCallStruct*>(&expr.code.data[i + 2]);
	callStruct->call(registers, (int)rnfirst);
	i += sizeof(DummyCallStruct);
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
	registers[rnsecond] = (void*)((char*)registers[rnfirst] + *reinterpret_cast<short int*>(&expr.code.data[i + 2]));
	i += sizeof(short int);
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
	*(int*)&registers[rnfirst] += *(int*)&registers[rnsecond];
	break;
}
case opcode::sub_int_int://int - int
{
	*(int*)&registers[rnfirst] -= *(int*)&registers[rnsecond];
	break;
}
case opcode::mul_int_int://int * int
{
	*(int*)&registers[rnfirst] *= *(int*)&registers[rnsecond];
	break;
}
case opcode::div_int_int://int / int
{
	*(int*)&registers[rnfirst] /= *(int*)&registers[rnsecond];
	break;
}

case opcode::int_to_float://int / int
{
	*(float*)&registers[rnfirst] = *(int*)&registers[rnsecond];
	break;
}

case opcode::add_float_float://float + float
{
	*(float*)&registers[rnfirst] += *(float*)&registers[rnsecond];
	break;
}
case opcode::sub_float_float://float - float
{
	*(float*)&registers[rnfirst] -= *(float*)&registers[rnsecond];
	break;
}
case opcode::mul_float_float://float * float
{
	*(float*)&registers[rnfirst] *= *(float*)&registers[rnsecond];
	break;
}
case opcode::div_float_float://float / float
{
	*(float*)&registers[rnfirst] /= *(float*)&registers[rnsecond];
	break;
}

case opcode::inc:
{
	(*(int*)registers[rnsecond])++;
	break;
}
case opcode::dec:
{
	(*(int*)registers[rnsecond])--;
	break;
}