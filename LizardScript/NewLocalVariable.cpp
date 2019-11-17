#include "ByteCodeGenerator.h"
#include "FindType.h"

using namespace LizardScript;

bool ByteCodeGenerator::open_reg(typed_reg& r, int ptrLevel)
{
	while (r.type.ptr > ptrLevel)
	{
		if (r.type.openedSize() <= 4)
		{
			code << opcode::get_32 << r; r.type.ptr--;
		}
		else if (r.type.openedSize() <= 8)
		{
			code << opcode::get_64 << r; r.type.ptr--;
		}
		else
		//if (r.type.size() > sizeof(void*))
		{
			r.type.ptr++;
			throw Exception(std::string("Type ") + r.type.text() + " can't be placed in a register.");
			return false;
		}
	}
	return true;
}

FieldInfo& ByteCodeGenerator::newLocalVariable(TCHAR* type, const TCHAR* name)
{
	return newLocalVariable(findType(type), name);
}

FieldInfo& ByteCodeGenerator::newLocalVariable(TypeInfo info, const TCHAR* name, bool getAddressImmediately)
{
	FieldInfo v;
	v.name = std::string(name);

	v.type = info;
	v.offset = localVarOffset;
	localVarOffset += v.type.size();

	if (localVarOffset > localVarMaxOffset)
		localVarMaxOffset = localVarOffset;

	if (getAddressImmediately)
	{
		auto& r = reg.alloc(v.type);
		r.type.ptr++;
		code << opcode::push_stackbase << r;

		//if (v.offset != 0)
		code << opcode::push_offset << r << (short int)v.offset;
	}

	localVar.push_back(v);
	return localVar[localVar.size() - 1];
}