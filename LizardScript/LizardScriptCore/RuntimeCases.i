#line 1 "RuntimeCases.h"
#line 1 "c:\\work_2020\\lizardscript\\lizardscript\\lizardscriptcore\\RuntimeMacros.h"
#pragma once

			//int += float;





			//int = int / float;





			//int += int;





			//int = int / int;










//int = float













































#line 2 "RuntimeCases.h"
#line 1 "c:\\work_2020\\lizardscript\\lizardscript\\lizardscriptcore\\LsTypedefs.h"
#pragma once












#line 3 "RuntimeCases.h"

case LsAsm::push_int: { CODEGET(OFFSET_T, R1);CODEGET(int, P_VALUE);*REGISTER(int, CODE(R1)) = CODE(P_VALUE);; break; }

case LsAsm::add_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) + *REGISTER(int, CODE(R2)); break; }case LsAsm::sub_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) - *REGISTER(int, CODE(R2)); break; }case LsAsm::mul_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) * *REGISTER(int, CODE(R2)); break; }case LsAsm::div_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) / *REGISTER(int, CODE(R2)); break; }
case LsAsm::add_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) + *REGISTER(float, CODE(R2)); break; }case LsAsm::sub_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) - *REGISTER(float, CODE(R2)); break; }case LsAsm::mul_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) * *REGISTER(float, CODE(R2)); break; }case LsAsm::div_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) / *REGISTER(float, CODE(R2)); break; }
case LsAsm::more_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) > *REGISTER(int, CODE(R2)); break; }case LsAsm::less_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) < *REGISTER(int, CODE(R2)); break; }case LsAsm::more_eq_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) >= *REGISTER(int, CODE(R2)); break; }case LsAsm::less_eq_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) <= *REGISTER(int, CODE(R2)); break; }case LsAsm::eq_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) == *REGISTER(int, CODE(R2)); break; }case LsAsm::not_eq_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) != *REGISTER(int, CODE(R2)); break; }
case LsAsm::more_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) > *REGISTER(float, CODE(R2)); break; }case LsAsm::less_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) < *REGISTER(float, CODE(R2)); break; }case LsAsm::more_eq_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) >= *REGISTER(float, CODE(R2)); break; }case LsAsm::less_eq_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) <= *REGISTER(float, CODE(R2)); break; }case LsAsm::eq_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) == *REGISTER(float, CODE(R2)); break; }case LsAsm::not_eq_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) != *REGISTER(float, CODE(R2)); break; }

case LsAsm::and_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) & *REGISTER(int, CODE(R2)); break; }case LsAsm::or_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) | *REGISTER(int, CODE(R2)); break; }case LsAsm::xor_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) ^ *REGISTER(int, CODE(R2)); break; }case LsAsm::not_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = ~ *REGISTER(int, CODE(R2)); break; }

case LsAsm::cast_int_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(float, CODE(R2)); break; }
case LsAsm::cast_float_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(int, CODE(R2)); break; }

case LsAsm::cmp_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) == *REGISTER(int, CODE(R2)); break; }
case LsAsm::mov_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R2)); break; }

case LsAsm::jmp: { CODEGET(int, new_eip); JMP(CODE(new_eip)); break; };
case LsAsm::jt: { CODEGET(OFFSET_T, R1); CODEGET(LsInternalAddr, P_ADDRESS); if (*REGISTER(int, CODE(R1)) != 0) JMP(CODE(P_ADDRESS)); break; };
case LsAsm::jf: { CODEGET(OFFSET_T, R1); CODEGET(LsInternalAddr, P_ADDRESS); if (*REGISTER(int, CODE(R1)) == 0) JMP(CODE(P_ADDRESS)); break; };

case LsAsm::wait: { system("pause"); break; };
case LsAsm::out_int: { CODEGET(OFFSET_T, R1); std::cout << *REGISTER(int, CODE(R1)) << std::endl; break; };
case LsAsm::out_float: { CODEGET(OFFSET_T, R1); std::cout << *REGISTER(float, CODE(R1)) << std::endl; break; };
case LsAsm::ret: { goto end; break; };
