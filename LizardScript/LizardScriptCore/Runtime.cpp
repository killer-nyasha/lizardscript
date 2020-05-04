#include "pch.h"
#include <iostream>

#include "Runtime.h"
#include "LsTypedefs.h"
#include "RuntimeMacros.h"
#include "Exception.h"

#include "Opcodes.hxx"

using namespace LizardScript;
using namespace LsAsm;

void Runtime::run(const LsFunction& f)
{
	//4 ����� - last frame LsFunction
	//4 ����� - last frame EIP
	//4 ����� - last frame ESP			
	//����� ���������
	//����� ��������
	//����� ��������� ����������

	//short frame mode - r1 � r2 �� 1 �����, �������� 256 ���������� ���� �� ���� ����� � ������
	//long frame mode - r1 � r2 �� 2 �����, �� 65536 ���� �� ���� ��� ������

	size_t eip = 0;
	size_t esp = 0;

	using OFFSET_T = unsigned char;

#define REGISTER(type, i) reinterpret_cast<type*>(&stack[esp + i])
	//r == 0 ? *(type*)((char*)stackbase + CODEGET(short)) : *(type*)&(registers[r])
#define CODEGET(type) *(type*)(&f.code[(eip += sizeof(type)) - sizeof(type)])

	while (true)
	{
		LsCode code = CODEGET(LsCode);
		OFFSET_T r1 = CODEGET(OFFSET_T);
		OFFSET_T r2 = CODEGET(OFFSET_T);

		switch (code)
		{
#include "RuntimeCases.i"

		default:
			throw Exception();
		}
	}

end:;

}
