#pragma once

			//int += float;
#define OPCODE_OPERATOR2(T1, T2, OPERATOR)\
*REGISTER(T1, r1) OPERATOR *REGISTER(T1, r2)

			//int = int / float;
#define OPCODE_OPERATOR3(T1, T2, TR, OPERATOR)\
*REGISTER(TR, r1) = *REGISTER(T1, r1) OPERATOR *REGISTER(T1, r2)

			//int += int;
#define OPCODE_ST_OPERATOR2(T, OPERATOR)\
*REGISTER(T, r1) OPERATOR *REGISTER(T, r2)

			//int = int / int;
#define OPCODE_ST_OPERATOR3(T, OPERATOR)\
*REGISTER(T, r1) = *REGISTER(T, r1) OPERATOR *REGISTER(T, r2)

#define OPCODE_PUSH_CONST(T)\
*REGISTER(T, r2) = CODEGET(T)

#define CASE_OPERATOR2(T1, T2, OPERATOR, NAME)\
case LsAsm::##NAME##_##T1##_T2: { OPCODE_OPERATOR2(T1, T2, OPERATOR); break; }

#define CASE_OPERATOR3(T1, T2, TR, OPERATOR, NAME)\
case LsAsm::##NAME##_##T1##_T2##_##TR: { OPCODE_OPERATOR3(T1, T2, TR, OPERATOR); break; }

#define CASE_ST_OPERATOR2(T, OPERATOR, NAME)\
case LsAsm::##NAME##_##T: { OPCODE_ST_OPERATOR2(T, OPERATOR); break; }

#define CASE_ST_OPERATOR3(T, OPERATOR, NAME)\
case LsAsm::##NAME##_##T: { OPCODE_ST_OPERATOR3(T, OPERATOR); break; }

#define MULTICASE_ST_ARITHMETICS(T)\
CASE_ST_OPERATOR3(T, +, add)\
CASE_ST_OPERATOR3(T, -, sub)\
CASE_ST_OPERATOR3(T, *, mul)\
CASE_ST_OPERATOR3(T, /, div)\

#define OPCODE_CAST(T1, T2)\
*REGISTER(T1, r1) = *REGISTER(T1, r1)

#define CASE_CAST(T1, T2)\
case LsAsm::cast_##T1##_##T2: { OPCODE_CAST(T1, T2); break; } 

#define OPCODE_PUSH(T)\
*REGISTER(T, r1) = CODEGET(T)

#define CASE_PUSH(T)\
case LsAsm::push_##T: { OPCODE_PUSH(T); break; }