#include "RuntimeMacros.h"
#include "LsTypedefs.h"

CASE_PUSH(int)
MULTICASE_ST_ARITHMETICS(int)
MULTICASE_ST_ARITHMETICS(float)
CASE_CAST(int, uint)
CASE_CAST(int, float)
CASE_CAST(float, int)
CASE_CUSTOM(out_int, std::cout << *REGISTER(int, r1) << std::endl)
CASE_CUSTOM(out_float, std::cout << *REGISTER(float, r1) << std::endl)
CASE_CUSTOM(ret, goto end)

CASE_CUSTOM(jmp, eip = CODEGET(int))
CASE_CUSTOM(jt, int new_eip = CODEGET(int); if (*REGISTER(int, r1) != 0) eip = new_eip)
CASE_CUSTOM(jf, int new_eip = CODEGET(int); if (*REGISTER(int, r1) == 0) eip = new_eip)

CASE_CUSTOM(wait, system("pause"))

CASE_CUSTOM(cmp, *REGISTER(int, r1) = *REGISTER(int, r1) == *REGISTER(int, r2))

CASE_ST_OPERATOR2(int, =, mov)