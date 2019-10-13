#include "pch.h"
#include <iostream>
#include <fstream>

#include <Windows.h>

#include "nasm_as_lib_import.h"

#pragma comment(lib, "nasm.lib")

#include "Lightspeed.h"
#include "../LizardScript/LizardScriptLibrary.h"

void dummy() { }

#define NASM_DATA_SIZE 0x13988
#define NASM_DATA_OFFSET 0x169000

Lightspeed::Lightspeed(size_t dcodeSize, size_t asmBufferSize) : dcodeSize(dcodeSize), asmBufferSize(asmBufferSize)
{
	dcode = new char[dcodeSize];
	asmBufferPtr = asmBuffer = new char[asmBufferSize];
	nasmDump = new char[0x13988];
	pNasmDll = (char*)GetModuleHandleA("nasm.dll");
}

Lightspeed::~Lightspeed()
{
	delete[] asmBuffer;
	delete[] nasmDump;
	setProtection(false);
	delete[] dcode;
}

const char* getRegName(regindex n, bool& isReg)
{
	const char* ret = nullptr;
	isReg = true;

	switch (n)
	{
		case 1: ret = "eax"; break;
		case 2: ret = "ecx"; break;
		case 3: ret = "edx"; break;
		case 4: ret = "ebx"; break;
		case 5: ret = "esi"; break;
		case 6: ret = "edi"; break;
		case 7: ret = "ebp"; break;
		default:
		{
			isReg = false;
			ret = "";//???
			break;
		}
	}

	return ret;
}

void Lightspeed::operator<<(Expr& expr)
{
	int stackAlignment = 16;
	if (expr.maxStackSize % stackAlignment != 0)
		expr.maxStackSize += stackAlignment - expr.maxStackSize % stackAlignment;

	line("sub rsp, ", expr.maxStackSize);

	for (size_t i = 0; i < expr.code.data.size(); i++)
	{
		regindex_pair rn = *reinterpret_cast<regindex_pair*>(&expr.code.data[i + 1]);
		regindex rnfirst = rn.first;
		regindex rnsecond = rn.second;
		switch ((opcode)expr.code.data[i])
		{
//#include "opcodes.h"

			case opcode::add_int_int:
			{
				line("add ", rn.first, ", ", rn.second);
				break;
			}

			default:
			{
				throw Exception("Unknown opcode " + std::to_string(*(unsigned char*)&expr.code.data[i]) + " at " + std::to_string(i));
			}
		}

	}

	line("add rsp, ", expr.maxStackSize);
}

void Lightspeed::assemble()
{
	memcpy(nasmDump, pNasmDll + NASM_DATA_OFFSET, NASM_DATA_SIZE);

	std::string path = std::string("..\\LsLightspeed\\dcode.asm");
	char* args[2];
	args[0] = nullptr;
	args[1] = &path[0];
	inputBuffer = inputBufferStart = asmBuffer;
	outputBuffer = outputBufferStart = dcode;
	mainAsLib(2, args);

	memcpy(pNasmDll + NASM_DATA_OFFSET, nasmDump, NASM_DATA_SIZE);
}

void Lightspeed::setProtection(bool isExecutable)
{
	DWORD oldP;
	if (isExecutable)
		VirtualProtect(dcode, dcodeSize, PAGE_EXECUTE_READWRITE, &oldP);
	else 
		VirtualProtect(dcode, dcodeSize, PAGE_READWRITE, &oldP);
}

	//std::ifstream fs("..\\LsLightspeed\\dcode", std::ios::binary);
	//fs.seekg(0, std::ios::end);
	//int length = fs.tellg();
	//fs.seekg(0, std::ios::beg);
	//fs.read(dcode, length);

	
	//char* i;


	//char* ch1 = (char*)"use32\0mov ecx, 2\0ret\0";
	//char* ch2 = (char*)"use32\0mov eax, 1\0ret\0";

	//char* pNasmDll = (char*)GetModuleHandleA("nasm.dll");
	//char copy[0x13988];
	//memcpy(copy, pNasmDll + 0x169000, 0x13988);

	//i = &ch1[0];

	/*
	   SECTION HEADER #3
   .data name
   13988 virtual size
  169000 virtual address (10169000 to 1017C987)
	 C00 size of raw data
  167200 file pointer to raw data (00167200 to 00167DFF)
	   0 file pointer to relocation table
	   0 file pointer to line numbers
	   0 number of relocations
	   0 number of line numbers
C0000040 flags
		 Initialized Data
		 Read Write
	*/


	//memcpy(pNasmDll + 0x169000, copy, 0x13988);

	//args[0] = nullptr;
	//args[1] = &path[0];
	//i = &ch2[0];
	//inputBuffer = inputBufferStart = i;
	//outputBuffer = outputBufferStart = dcode;
	//mainAsLib(2, args);