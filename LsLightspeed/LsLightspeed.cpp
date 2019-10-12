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
	std::string l1 = std::string("mov ecx, 2");
	std::string l2 = std::string("mov eax, 1");

	//std::string buf;
	//buf = path;

	args[0] = nullptr;
	args[1] = &path[0];
	i = &l1[0];
	inputBuffer = inputBufferStart = i;
	outputBuffer = outputBufferStart = dcode;
	mainAsLib(2, args);

	args[0] = nullptr;
	args[1] = &path[0];
	i = &l2[0];
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