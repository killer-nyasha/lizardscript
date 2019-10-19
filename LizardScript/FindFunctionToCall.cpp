#include "ByteCodeGenerator.h"
#include "FindType.h"

using namespace LizardScript;

void ByteCodeGenerator::findFunctionToCall(PossibleFunctionCalls& call)
{
	int csize = code.data.size();

	std::vector<typed_reg> args;
	int lastArgIndex = reg.stackEmul.top().index;
	for (size_t i = 0; i < lastArgIndex - call.index; i++)
		args.push_back(reg.free());
	for (size_t i = 0; i < args.size() / 2; i++)
		std::swap(args[i], args[args.size() - i - 1]);

	typed_reg __ths = reg.free();

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

		for (size_t i = 0; i < args.size(); i++)
			open_reg(args[i], f->args[i].ptr);
		code << opcode::call_cpp << call.index;
		for (size_t i = 0; i < sizeof(f->callStruct); i++)
			code << f->callStruct[i];

		//а что если аргументы в неправильном порядке????!!!

		if (!(f->type == makeTypeInfo<void>()))
		{
			typed_reg returnValue = reg.alloc(f->type);

			//reg.stackEmul
		}
		return;
	}

	throw Exception(std::string("All \"") + call.functions[0]->name + "\" overloads were insuitable.");
}