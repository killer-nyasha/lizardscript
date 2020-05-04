#include "RuntimeMacros.h"
#include "LsTypedefs.h"

CASE_PUSH(int)
MULTICASE_ST_ARITHMETICS(int)
CASE_CAST(int, uint)
CASE_CUSTOM(out, std::cout << *REGISTER(int, r1))
CASE_CUSTOM(ret, goto end)

