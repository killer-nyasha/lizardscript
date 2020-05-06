#include "RuntimeMacros.h"
#include "LsTypedefs.h"

CASE_PUSH(int)
MULTICASE_ST_ARITHMETICS(int)
MULTICASE_ST_ARITHMETICS(float)
CASE_CAST(int, uint)
CASE_CAST(int, float)
CASE_CAST(float, int)

CASE_CUSTOM(out_int, CODEGET(OFFSET_T, R1); std::cout << *REGISTER(int, CODE(R1)) << std::endl)
CASE_CUSTOM(out_float, CODEGET(OFFSET_T, R1); std::cout << *REGISTER(float, CODE(R1)) << std::endl)
CASE_CUSTOM(ret, goto end)

CASE_CUSTOM(jmp, CODEGET(int, new_eip); JMP(CODE(new_eip)))
CASE_CUSTOM(jt, CODEGET(OFFSET_T, R1); CODEGET(LsInternalAddr, P_ADDRESS); if (*REGISTER(int, CODE(R1)) != 0) JMP(CODE(P_ADDRESS)))
CASE_CUSTOM(jf, CODEGET(OFFSET_T, R1); CODEGET(LsInternalAddr, P_ADDRESS); if (*REGISTER(int, CODE(R1)) == 0) JMP(CODE(P_ADDRESS)))

CASE_CUSTOM(wait, system("pause"))

//CASE_CUSTOM(cmp, *REGISTER(int, R1) = *REGISTER(int, R1) == *REGISTER(int, R2))

CASE_ST_OPERATOR3(int, ==, cmp)
CASE_ST_OPERATOR2(int, =, mov)