#line 1 "RuntimeCases.h"
#line 1 "c:\\work_2020\\lizardscript\\lizardscript\\lizardscriptcore\\RuntimeMacros.h"
#pragma once

			//int += float;





			//int = int / float;





			//int += int;





			//int = int / int;















//int = float





















































#line 2 "RuntimeCases.h"
//#include "LsTypedefs.h"

case LsAsm::nop: { ;; break; };

case LsAsm::push_float:
case LsAsm::push_int: { CODEGET(OFFSET_T, R1);CODEGET(int, P_VALUE);*REGISTER(int, CODE(R1)) = CODE(P_VALUE);; break; }
case LsAsm::push_int64: { CODEGET(OFFSET_T, R1);CODEGET(int64, P_VALUE);*REGISTER(int64, CODE(R1)) = CODE(P_VALUE);; break; }

case LsAsm::pushstr_stdstring: { CODEGET(OFFSET_T, R1);CODEGETSTR(P_VALUE);new (REGISTER(stdstring, CODE(R1))) stdstring(CODE(P_VALUE));; break; }

case LsAsm::add_stdstring: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(stdstring, CODE(R1)) = *REGISTER(stdstring, CODE(R1)) + *REGISTER(stdstring, CODE(R2)); break; }

case LsAsm::add_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) + *REGISTER(int, CODE(R2)); break; }case LsAsm::sub_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) - *REGISTER(int, CODE(R2)); break; }case LsAsm::mul_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) * *REGISTER(int, CODE(R2)); break; }case LsAsm::div_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) / *REGISTER(int, CODE(R2)); break; }
case LsAsm::more_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) > *REGISTER(int, CODE(R2)); break; }case LsAsm::less_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) < *REGISTER(int, CODE(R2)); break; }case LsAsm::more_eq_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) >= *REGISTER(int, CODE(R2)); break; }case LsAsm::less_eq_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) <= *REGISTER(int, CODE(R2)); break; }case LsAsm::eq_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) == *REGISTER(int, CODE(R2)); break; }case LsAsm::not_eq_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) != *REGISTER(int, CODE(R2)); break; }

case LsAsm::add_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) + *REGISTER(float, CODE(R2)); break; }case LsAsm::sub_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) - *REGISTER(float, CODE(R2)); break; }case LsAsm::mul_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) * *REGISTER(float, CODE(R2)); break; }case LsAsm::div_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) / *REGISTER(float, CODE(R2)); break; }
case LsAsm::more_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) > *REGISTER(float, CODE(R2)); break; }case LsAsm::less_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) < *REGISTER(float, CODE(R2)); break; }case LsAsm::more_eq_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) >= *REGISTER(float, CODE(R2)); break; }case LsAsm::less_eq_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) <= *REGISTER(float, CODE(R2)); break; }case LsAsm::eq_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) == *REGISTER(float, CODE(R2)); break; }case LsAsm::not_eq_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(float, CODE(R1)) != *REGISTER(float, CODE(R2)); break; }

case LsAsm::add_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int64, CODE(R1)) + *REGISTER(int64, CODE(R2)); break; }case LsAsm::sub_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int64, CODE(R1)) - *REGISTER(int64, CODE(R2)); break; }case LsAsm::mul_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int64, CODE(R1)) * *REGISTER(int64, CODE(R2)); break; }case LsAsm::div_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int64, CODE(R1)) / *REGISTER(int64, CODE(R2)); break; }
case LsAsm::more_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int64, CODE(R1)) > *REGISTER(int64, CODE(R2)); break; }case LsAsm::less_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int64, CODE(R1)) < *REGISTER(int64, CODE(R2)); break; }case LsAsm::more_eq_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int64, CODE(R1)) >= *REGISTER(int64, CODE(R2)); break; }case LsAsm::less_eq_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int64, CODE(R1)) <= *REGISTER(int64, CODE(R2)); break; }case LsAsm::eq_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int64, CODE(R1)) == *REGISTER(int64, CODE(R2)); break; }case LsAsm::not_eq_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int64, CODE(R1)) != *REGISTER(int64, CODE(R2)); break; }

case LsAsm::and_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) & *REGISTER(int, CODE(R2)); break; }case LsAsm::or_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) | *REGISTER(int, CODE(R2)); break; }case LsAsm::xor_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R1)) ^ *REGISTER(int, CODE(R2)); break; }case LsAsm::not_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = ~ *REGISTER(int, CODE(R2)); break; }
case LsAsm::and_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int64, CODE(R1)) & *REGISTER(int64, CODE(R2)); break; }case LsAsm::or_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int64, CODE(R1)) | *REGISTER(int64, CODE(R2)); break; }case LsAsm::xor_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int64, CODE(R1)) ^ *REGISTER(int64, CODE(R2)); break; }case LsAsm::not_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = ~ *REGISTER(int64, CODE(R2)); break; }

case LsAsm::cast_int_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(float, CODE(R2)); break; }
case LsAsm::cast_float_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(int, CODE(R2)); break; }
case LsAsm::cast_int64_float: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(float, CODE(R2)); break; }
case LsAsm::cast_float_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(float, CODE(R1)) = *REGISTER(int64, CODE(R2)); break; }
case LsAsm::cast_int_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int64, CODE(R2)); break; }
case LsAsm::cast_int64_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int, CODE(R2)); break; }

case LsAsm::mov_float:
case LsAsm::mov_int: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int, CODE(R1)) = *REGISTER(int, CODE(R2)); break; }
case LsAsm::mov_int64: { CODEGET(OFFSET_T, R1);CODEGET(OFFSET_T, R2);*REGISTER(int64, CODE(R1)) = *REGISTER(int64, CODE(R2)); break; }

case LsAsm::jmp: { CODEGET(int, new_eip); JMP(CODE(new_eip)); break; };
case LsAsm::jt: { CODEGET(OFFSET_T, R1); CODEGET(LsInternalAddr, P_ADDRESS); if (*REGISTER(int, CODE(R1)) != 0) JMP(CODE(P_ADDRESS)); break; };
case LsAsm::jf: { CODEGET(OFFSET_T, R1); CODEGET(LsInternalAddr, P_ADDRESS); if (*REGISTER(int, CODE(R1)) == 0) JMP(CODE(P_ADDRESS)); break; };

case LsAsm::wait: { crossplatform_pause(); break; };
case LsAsm::out_int: { CODEGET(OFFSET_T, R1); std::cout << *REGISTER(int, CODE(R1)) << std::endl; break; }
case LsAsm::out_float: { CODEGET(OFFSET_T, R1); std::cout << *REGISTER(float, CODE(R1)) << std::endl; break; }
case LsAsm::out_int64: { CODEGET(OFFSET_T, R1); std::cout << *REGISTER(int64, CODE(R1)) << std::endl; break; }
case LsAsm::out_stdstring: { CODEGET(OFFSET_T, R1); std::cout << *REGISTER(stdstring*, CODE(R1)) << std::endl; break; };

case LsAsm::ret: { goto end; break; };
