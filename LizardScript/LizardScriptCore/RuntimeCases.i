#line 1 "RuntimeCases.h"
#line 1 "c:\\work_2020\\lizardscript\\lizardscript\\lizardscriptcore\\RuntimeMacros.h"
#pragma once







			



			



			



			





























#line 2 "RuntimeCases.h"
#line 1 "c:\\work_2020\\lizardscript\\lizardscript\\lizardscriptcore\\LsTypedefs.h"
#pragma once












#line 3 "RuntimeCases.h"

case add_int: { *reinterpret_cast<int*>(&stack[esp + r1]) = *reinterpret_cast<int*>(&stack[esp + r1]) + *reinterpret_cast<int*>(&stack[esp + r2]); break; }case sub_int: { *reinterpret_cast<int*>(&stack[esp + r1]) = *reinterpret_cast<int*>(&stack[esp + r1]) - *reinterpret_cast<int*>(&stack[esp + r2]); break; }case mul_int: { *reinterpret_cast<int*>(&stack[esp + r1]) = *reinterpret_cast<int*>(&stack[esp + r1]) * *reinterpret_cast<int*>(&stack[esp + r2]); break; }case div_int: { *reinterpret_cast<int*>(&stack[esp + r1]) = *reinterpret_cast<int*>(&stack[esp + r1]) / *reinterpret_cast<int*>(&stack[esp + r2]); break; }
case cast_int_uint: { *reinterpret_cast<int*>(&stack[esp + r1]) = *reinterpret_cast<int*>(&stack[esp + r1]); break; }
