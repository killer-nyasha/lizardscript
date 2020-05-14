#include "RuntimeMacros.h"
//#include "LsTypedefs.h"

CASE_CUSTOM(nop, ;)

CASE_ALIAS(push_float)
CASE_PUSH(int)
CASE_PUSH(int64)

CASE_PUSHSTR(stdstring)

CASE_ST_OPERATOR3(stdstring, +, add)

MULTICASE_ST_ARITHMETICS(int)
MULTICASE_ST_COMPARISON(int)

MULTICASE_ST_ARITHMETICS(float)
MULTICASE_ST_COMPARISON(float)

MULTICASE_ST_ARITHMETICS(int64)
MULTICASE_ST_COMPARISON(int64)

MULTICASE_ST_LOGIC(int)
MULTICASE_ST_LOGIC(int64)

CASE_CAST(int, float)
CASE_CAST(float, int)
CASE_CAST(int64, float)
CASE_CAST(float, int64)
CASE_CAST(int, int64)
CASE_CAST(int64, int)

CASE_ALIAS(mov_float)
CASE_ST_OPERATOR2(int, =, mov)
CASE_ST_OPERATOR2(int64, =, mov)

CASE_CUSTOM(jmp, CODEGET(int, new_eip); JMP(CODE(new_eip)))
CASE_CUSTOM(jt, CODEGET(OFFSET_T, R1); CODEGET(LsInternalAddr, P_ADDRESS); if (*REGISTER(int, CODE(R1)) != 0) JMP(CODE(P_ADDRESS)))
CASE_CUSTOM(jf, CODEGET(OFFSET_T, R1); CODEGET(LsInternalAddr, P_ADDRESS); if (*REGISTER(int, CODE(R1)) == 0) JMP(CODE(P_ADDRESS)))

CASE_CUSTOM(wait, crossplatform_pause())
CASE_OUT(int)
CASE_OUT(float)
CASE_OUT(int64)
CASE_CUSTOM(out_stdstring, CODEGET(OFFSET_T, R1); std::cout << *REGISTER(stdstring*, CODE(R1)) << std::endl)

CASE_CUSTOM(ret, goto end)