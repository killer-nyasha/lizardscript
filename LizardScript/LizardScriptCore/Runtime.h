#pragma once
#include <vector>

namespace LizardScript
{
	using LsCode = unsigned char;

	class LsFunction
	{
	public:
		std::vector<unsigned char> code;
	};

	class Runtime
	{
	public:

		unsigned char stack[1024 * 10];

		void run(const LsFunction& f)
		{
			//4 байта - last frame LsFunction
			//4 байта - last frame EIP
			//4 байта - last frame ESP			
			//далее параметры
			//далее регистры
			//далее локальные переменные

			//short frame mode - r1 и r2 по 1 байту, максимум 256 адресуемых байт на кадр стека и объект
			//long frame mode - r1 и r2 по 2 байта, до 65536 байт на кадр или объект

			size_t eip = 0;
			size_t esp = 0;

			using OFFSET_T = unsigned char;

#define REGISTER(type, i) = stack[esp + i];
			//r == 0 ? *(type*)((char*)stackbase + CODEGET(short)) : *(type*)&(registers[r])
#define CODEGET(type) *(type*)(&f.code[(eip += sizeof(type)) - sizeof(type)])
#define PCODEGET(type) (type*)(&f.code[(eip += sizeof(type)) - sizeof(type)])

#define OPCODE_SET(T1, T2)\
*REGISTER(T1*, r1) = REGISTER(T1, r2);

#define OPCODE_PUSH_CONST(T)\
*REGISTER(T*, r2) = CODEGET(T);

			while (true)
			{
				LsCode code = CODEGET(LsCode);
				OFFSET_T r1 = CODEGET(OFFSET_T);
				OFFSET_T r2 = CODEGET(OFFSET_T);


			}
		}
	};
}