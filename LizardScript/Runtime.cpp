#define _CRT_SECURE_NO_WARNINGS

#include <chrono>

#include "Runtime.h"
#include "LizardScriptLibrary.h"

//#include "../Sstp/stringptr.h"

using namespace LizardScript;

char* global_stack = new char[1024];


void Runtime::run(bool catchEx)
{
	if (catchEx)
	try
	{
		run_impl();
	}
	catch (Exception& ex)
	{
		//logger.add("Runtime error: \"", ex.text, "\"");
		std::cout << COLOR_RED << "Runtime error: \"" << ex.text << "\"" << COLOR_NC << std::endl;
	}
	catch (...)
	{
		//logger.add("Unknown runtime C++ error");
		std::cout << "Unknown runtime C++ error" << std::endl;
	}
	else
	{
		run_impl();
	}
}

void Runtime::run_impl()
{
	memset(registers, 0, sizeof(registers));
	void*& stackptr = registers[16];
	void*& stackbase = registers[17];
	global_stack += expr.maxStackSize;
	stackbase = stackptr = global_stack;
	//std::cout << expr.maxStackSize << std::endl;
	memset(stackbase, 0, expr.maxStackSize);
	stackptr = (char*)stackptr + expr.maxStackSize;

	//global_stack += expr.maxStackSize;
	//alloca(expr.maxStackSize+0x08);


	//void* stackbase = stackptr;

	//размер регистра сейчас зависит от разрядности. не должен??
	//registers[16] = stackdata;

	size_t i = 0;
	//volatile int k = 0;
	//try
	{
		//if (lsl.printDump)
		//	expr.disasm();

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
		unsigned char* data = (unsigned char*)&expr.code.data[0];
		for (; /*i < expr.code.data.size()*/; )
		{
			//std::cout << i << ": " << *(long long int*)stackdata << ";" << std::endl;

			opcode op = (opcode)data[i];
			regindex_pair rn = *reinterpret_cast<regindex_pair*>(&data[i + 1]);
			regindex rnfirst = rn.first;
			regindex rnsecond = rn.second;
			i += 2;

			switch (op)
			{
#include "opcodes.h"

			default:
			{
				throw Exception("Unknown opcode " + std::to_string(*(unsigned char*)&data[i]) + " at " + std::to_string(i));
			}
			}

		}

		end:

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

	global_stack -= expr.maxStackSize;
}
