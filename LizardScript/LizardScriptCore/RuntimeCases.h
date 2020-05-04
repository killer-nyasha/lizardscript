#include "RuntimeMacros.h"
#include "LsTypedefs.h"

CASE_PUSH(int)
MULTICASE_ST_ARITHMETICS(int)
MULTICASE_ST_ARITHMETICS(float)
CASE_CAST(int, uint)
CASE_CAST(int, float)
CASE_CAST(float, int)
CASE_CUSTOM(out_int, std::cout << *REGISTER(int, r1))
CASE_CUSTOM(out_float, std::cout << *REGISTER(float, r1))
CASE_CUSTOM(ret, goto end)

