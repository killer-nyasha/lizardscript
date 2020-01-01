#include "stdafx.h"
#include "ByteCodeGenerator.h"
#include "FindType.h"

using namespace LizardScript;

void ByteCodeGenerator::findFunctionToCall(PossibleFunctionCalls& call)
{
	int csize = code.data.size();
	typed_reg* rret = nullptr;
	FieldInfo fld;

	typed_reg _fld = reg.alloc(TYPEINFO(void));
	reg.stackEmul.pop();

	std::vector<typed_reg> args;
	int lastArgIndex = reg.stackEmul.top().index;
	for (size_t i = 0; i < lastArgIndex - call.index; i++)
		args.push_back(reg.stackEmul.top()), reg.stackEmul.pop();
	for (size_t i = 0; i < args.size() / 2; i++)
		std::swap(args[i], args[args.size() - i - 1]);

	typed_reg __ths = reg.stackEmul.top(); reg.stackEmul.pop();
	open_reg(__ths, 1);

	for (auto& f : call.functions)
	{
		if (args.size() != f->args.size())
			goto cont;

		for (size_t i = 0; i < args.size(); i++)
		{
			if (!cast(args[i], f->args[i]))
			{
				//WARNING(std::string("argument ") + std::to_string(i + 1) + ": "
				//	+ args[i].type.text() + " instead of " + f->args[i].text() + ".");
				code.data.resize(csize);
				goto cont;
			}
		}
		goto success;

	cont:
		continue;

	success:

		//for (size_t i = 0; i < args.size(); i++)
		//	open_reg(args[i], f->args[i].ptr);

		reg.stackEmul.push(__ths);
		for (size_t i = 0; i < args.size(); i++)
			reg.stackEmul.push(args[i]);
		reg.stackEmul.push(_fld);

		for (size_t i = 0; i < args.size() + 2; i++)
			reg.free();

		if (!(f->type == makeTypeInfo<void>()))
		{
			if (f->type.size() <= sizeof(void*))
			{
				typed_reg& returnValue = reg.alloc(f->type);
			}
			else
			{
				fld = newLocalVariable(f->type, "_temp_", false);

				rret = &reg.alloc(f->type);
				rret->type.ptr++;

				for (size_t i = localVar.size() - 1; i + 1 >= 1; i--)
				{
					if (_tcscmp(&localVar[i].name[0], "_temp_") == 0)
					{
						code.push(opcode::set_stackptr, regindex(), (short)localVar[i].offset);
						break;
					}
				}
			}
		}

		code << opcode::call_cpp << call.index;
		for (size_t i = 0; i < sizeof(f->callStruct); i++)
			code << ((char*)&f->callStruct)[i];


		if (!(f->type == makeTypeInfo<void>()))
		{
			if (f->type.size() <= sizeof(void*))
			{

			}
			else
			{
				//*****************
				code << opcode::push_stackbase << *rret;
				if (fld.offset != 0)
					code << opcode::push_offset << *rret << (short int)fld.offset;
				//*****************
			}
		}
		//а что если аргументы в неправильном порядке????!!!



		return;
	}

	std::string atypes;
	for (size_t i = 0; i < args.size(); i++)
	{
		atypes += args[i].type.text() + "; ";
	}

	throw Exception(std::string("All \"") + call.functions[0]->name + "\" overloads were insuitable for " + atypes);
}