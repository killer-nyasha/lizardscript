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
	args[0] = nullptr;
	args[1] = (char*)"..\\LsLightspeed\\dcode.asm";
	
	char* i;

	i = &std::string(" mov ecx, 2")[0];  
	inputBuffer = inputBufferStart = i;
	outputBuffer = outputBufferStart = dcode;
	mainAsLib(2, args);

	i = &std::string(" mov eax, 1")[0];
	inputBuffer = inputBufferStart = i;
	outputBuffer = outputBufferStart = dcode;
	mainAsLib(2, args);


	//char* c = outputBufferStart;

	DWORD oldP;
	VirtualProtect(dcode, 4096, PAGE_EXECUTE_READWRITE, &oldP);

	void(*pDummy)() = &dummy;
	char buf[sizeof(pDummy)];
	*reinterpret_cast<void(**)()>(&buf) = &dummy;
	*reinterpret_cast<void**>(&buf) = dcode;

	(*reinterpret_cast<void(**)()>(&buf))();
}