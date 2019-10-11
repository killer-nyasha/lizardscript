#include "pch.h"
#include <iostream>
#include <fstream>

#include <Windows.h>

char* dcode = new char[4096];

void dummy() { }

void lightspeed()
{
	std::ifstream fs("..\\LsLightspeed\\dcode", std::ios::binary);
	fs.seekg(0, std::ios::end);
	int length = fs.tellg();
	fs.seekg(0, std::ios::beg);
	fs.read(dcode, length);

	DWORD oldP;
	VirtualProtect(dcode, 4096, PAGE_EXECUTE_READWRITE, &oldP);

	void(*pDummy)() = &dummy;
	char buf[sizeof(pDummy)];
	*reinterpret_cast<void(**)()>(&buf) = &dummy;
	*reinterpret_cast<void**>(&buf) = dcode;

	(*reinterpret_cast<void(**)()>(&buf))();
}