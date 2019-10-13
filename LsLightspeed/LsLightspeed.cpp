#include "pch.h"
#include <iostream>
#include <fstream>

#include <Windows.h>

#include "nasm_as_lib_import.h"

#pragma comment(lib, "nasm.lib")

char* dcode = new char[4096];

void dummy() { }

void lightspeed()
{
	//std::ifstream fs("..\\LsLightspeed\\dcode", std::ios::binary);
	//fs.seekg(0, std::ios::end);
	//int length = fs.tellg();
	//fs.seekg(0, std::ios::beg);
	//fs.read(dcode, length);

	char* args[2];
	
	char* i;

	std::string path = std::string("..\\LsLightspeed\\dcode.asm");

	char* ch1 = (char*)"use32\0mov ecx, 2\0ret\0";
	char* ch2 = (char*)"use32\0mov eax, 1\0ret\0";

	//std::string buf;
	//buf = path;

	char* pNasmDll = (char*)GetModuleHandleA("nasm.dll");
	char copy[0x13988];
	memcpy(copy, pNasmDll + 0x169000, 0x13988);

	args[0] = nullptr;
	args[1] = &path[0];
	i = &ch1[0];
	inputBuffer = inputBufferStart = i;
	outputBuffer = outputBufferStart = dcode;
	mainAsLib(2, args);

	/*
       14000 .data
       D2000 .rdata
        E000 .reloc
       96000 .text

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

	//*(pNasmDll + 0x14000) = 'a';

	memcpy(pNasmDll + 0x169000, copy, 0x13988);

	//pNasmDll + 0x14000

	args[0] = nullptr;
	args[1] = &path[0];
	i = &ch2[0];
	inputBuffer = inputBufferStart = i;
	outputBuffer = outputBufferStart = dcode;
	mainAsLib(2, args);

	DWORD oldP;
	VirtualProtect(dcode, 4096, PAGE_EXECUTE_READWRITE, &oldP);

	void(*pDummy)() = &dummy;
	char buf[sizeof(pDummy)];
	*reinterpret_cast<void(**)()>(&buf) = &dummy;
	*reinterpret_cast<void**>(&buf) = dcode;

	(*reinterpret_cast<void(**)()>(&buf))();
}