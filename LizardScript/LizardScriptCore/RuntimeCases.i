#line 1 "RuntimeCases.h"
#line 1 "C:\\WORK_2020\\lizardscript\\LizardScript\\LizardScriptCore\\RuntimeMacros.h"
#pragma once







			



			



			



			





























#line 2 "RuntimeCases.h"
#line 1 "C:\\WORK_2020\\lizardscript\\LizardScript\\LizardScriptCore\\LsTypedefs.h"
#pragma once












#line 3 "RuntimeCases.h"

case add_int: { *stack[esp + r1] = *stack[esp + r1] + stack[esp + r2]; break; }case sub_int: { *stack[esp + r1] = *stack[esp + r1] - stack[esp + r2]; break; }case mul_int: { *stack[esp + r1] = *stack[esp + r1] * stack[esp + r2]; break; }case div_int: { *stack[esp + r1] = *stack[esp + r1] / stack[esp + r2]; break; }
case cast_int_uint: { *stack[esp + r1] = *stack[esp + r1]; break; }
