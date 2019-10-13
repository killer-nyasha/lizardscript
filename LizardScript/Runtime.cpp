#define _CRT_SECURE_NO_WARNINGS

#include <chrono>

#include "Runtime.h"
#include "LizardScriptLibrary.h"

//#include "../Sstp/stringptr.h"

using namespace LizardScript;

void Runtime::run()
{
	void* stackdata = alloca(expr.maxStackSize+0x08);
	memset(stackdata, 0, sizeof(stackdata));

	memset(registers, 0, sizeof(registers));
	size_t i = 0;
	//volatile int k = 0;
	//try
	{
		if (lsl.printDump)
			expr.disasm();

		//std::cin >> i;
		//int y = 1;

		std::chrono::high_resolution_clock::time_point t1 =
			std::chrono::high_resolution_clock::now();

		//if (true)
		//{
		//	int n = 100000000;
		//	int x = 1;
		//	int j = 2;
		//	while (j < n) {
		//		y = x + y;
		//		x = y - x;
		//		j++;
		//	}

		//}
		//else
		for (; i < expr.code.data.size(); i += 2)
		{
			//std::cout << i << ": " << *(long long int*)stackdata << ";" << std::endl;

			regindex_pair rn = *reinterpret_cast<regindex_pair*>(&expr.code.data[i + 1]);
			regindex rnfirst = rn.first;
			regindex rnsecond = rn.second;
			switch ((opcode)expr.code.data[i])
			{
#include "opcodes.h"

			default:
			{
				throw Exception("Unknown opcode " + std::to_string(*(unsigned char*)&expr.code.data[i]) + " at " + std::to_string(i));
			}
			}

		}

		std::chrono::high_resolution_clock::time_point t2 =
			std::chrono::high_resolution_clock::now();
		if (lsl.printRunTime)
			std::cout << COLOR_YELLOW << "run time: " << (t2 - t1).count() / 1000000.0 << " ms" << COLOR_NC << std::endl;
		//std::cout << k;
		//std::cout << y;
	}
	//catch (...)
	{
		//	std::cout << "i = " << i << std::endl;
		//	throw Exception("Ошибка во время выполнения скрипта. В консоль выведен лог с необходимой информацией.");
	}
}
