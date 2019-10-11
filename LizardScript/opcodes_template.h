#pragma once

//template <typename T1, typename T2>
//void opcode_set()
//{
//
//}

#define opcode_set(T1, T2)\
T2 right = *reinterpret_cast<T2*>(&registers[rnsecond]);\
T1* left = reinterpret_cast<T1*>(registers[rnfirst]);\
*left = static_cast<T2>(right);

#define opcode_push_const(T)\
T c = *reinterpret_cast<T*>(&expr.code.data[i+2]); \
*reinterpret_cast<T*>(&registers[rnsecond]) = c; \
i += sizeof(T)


//*reinterpret_cast<T*>(&r.registers[rnsecond]) = std::get<std::vector<T>>(r.expr.constants)[std::get<int_from<T>>(r.constantsIndex).value++];