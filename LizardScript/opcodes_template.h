#pragma once

//template <typename T1, typename T2>
//void opcode_set()
//{
//
//}

#define CODEGET(type) *(type*)(&data[(i += sizeof(type)) - sizeof(type)])
#define PCODEGET(type) (type*)(&data[(i += sizeof(type)) - sizeof(type)])

#define OPEN(type, r) (r == 0 ? *(type*)((char*)stackbase + CODEGET(short)) : *(type*)&(registers[r]))

#define opcode_set(T1, T2)\
*OPEN(T1*, rnfirst) = OPEN(T1, rnsecond);

#define opcode_push_const(T)\
OPEN(T, rnsecond) = CODEGET(T);


//*reinterpret_cast<T*>(&r.registers[rnsecond]) = std::get<std::vector<T>>(r.expr.constants)[std::get<int_from<T>>(r.constantsIndex).value++];