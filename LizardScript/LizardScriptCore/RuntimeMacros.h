#pragma once

#define REGISTER(type, i) stack[esp + i]
//r == 0 ? *(type*)((char*)stackbase + CODEGET(short)) : *(type*)&(registers[r])

#define CODEGET(type) *(type*)(&f.code[(eip += sizeof(type)) - sizeof(type)])
#define PCODEGET(type) (type*)(&f.code[(eip += sizeof(type)) - sizeof(type)])

			//int += float;
#define OPCODE_OPERATOR2(T1, T2, OPERATOR)\
*REGISTER(T1*, r1) OPERATOR REGISTER(T1, r2)

			//int = int / float;
#define OPCODE_OPERATOR3(T1, T2, TR, OPERATOR)\
*REGISTER(TR*, r1) = *REGISTER(T1*, r1) OPERATOR REGISTER(T1, r2)

			//int += int;
#define OPCODE_ST_OPERATOR2(T, OPERATOR)\
*REGISTER(T*, r1) OPERATOR REGISTER(T, r2)

			//int = int / int;
#define OPCODE_ST_OPERATOR3(T, OPERATOR)\
*REGISTER(T*, r1) = *REGISTER(T*, r1) OPERATOR REGISTER(T, r2)

#define OPCODE_PUSH_CONST(T)\
*REGISTER(T*, r2) = CODEGET(T)

#define CASE_OPERATOR2(T1, T2, OPERATOR, NAME)\
case NAME##_##T1##_T2: { OPCODE_OPERATOR2(T1, T2, OPERATOR); break; }

#define CASE_OPERATOR3(T1, T2, TR, OPERATOR, NAME)\
case NAME##_##T1##_T2##_##TR: { OPCODE_OPERATOR3(T1, T2, TR, OPERATOR); break; }

#define CASE_ST_OPERATOR2(T, OPERATOR, NAME)\
case NAME##_##T: { OPCODE_ST_OPERATOR2(T, OPERATOR); break; }

#define CASE_ST_OPERATOR3(T, OPERATOR, NAME)\
case NAME##_##T: { OPCODE_ST_OPERATOR3(T, OPERATOR); break; }

#define MULTICASE_ST_ARITHMETICS(T)\
CASE_ST_OPERATOR3(T, +, add)\
CASE_ST_OPERATOR3(T, -, sub)\
CASE_ST_OPERATOR3(T, *, mul)\
CASE_ST_OPERATOR3(T, /, div)\

#define OPCODE_CAST(T1, T2)\
*REGISTER(T1*, r1) = *REGISTER(T1*, r1)

#define CASE_CAST(T1, T2)\
case cast_##T1##_##T2: { OPCODE_CAST(T1, T2); break; } 