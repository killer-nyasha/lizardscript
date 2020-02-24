#include "pch.h"
#include "metagen_system.h"
#include "Print.h"
#include "ConsoleColor.h"

//using namespace LizardScript;

namespace LizardScript
{
	void print(std::ostream& stream, char* object, TypeInfo currentType)
	{
		auto& metatable = globalMetadataTable[currentType];

		stream << COLOR_BLUE << currentType.cppType.name() << COLOR_NC << " {\r\n";

		for (auto& metadata : metatable.members.get<FunctionInfo>())
		{
			stream << COLOR_BLUE << metadata.type.to_string() << " " << COLOR_NC << metadata.name << "(";
			bool needComma = false;
			for (auto& t : metadata.args)
			{
				if (!needComma)
					needComma = true;
				else stream << ", ";
				stream << COLOR_BLUE << t.to_string() << COLOR_NC;
			}
			stream << ");" << ENDL;
		}
		for (auto& metadata : metatable.members.get<FieldInfo>())
		{
			stream << COLOR_BLUE << metadata.type.to_string() << COLOR_NC << " " << metadata.name;

			if (metadata.type == typeInfo<int>())
			{
				int* p = (int*)(object + metadata.offset);
				for (size_t i = 0; i < metadata.type.ptr; i++)
					p = *((int**)p);
				stream << " = " << *p << ";" << ENDL;
			}
			else if (metadata.type == typeInfo<float>())
			{
				int* p = (int*)(object + metadata.offset);
				for (size_t i = 0; i < metadata.type.ptr; i++)
					p = *((int**)p);
				stream << " = " << *(float*)p << ";" << ENDL;
			}
			else if(metadata.type.ptr > 0)
			{
				char* objPtr = *(char**)(object + metadata.offset);

				stream << " = ";
				if (objPtr == nullptr)
					stream << COLOR_BLUE << "null;" << ENDL << COLOR_NC;
				else
				{
					//точно не нужно??
					//for (size_t i = 0; i < metadata.type.ptr; i++)
					//	objPtr = *((char**)objPtr);
					print(stream, objPtr, metadata.type);
				}
			}
			else
			{
				char* objPtr = (char*)(object + metadata.offset);
				stream << " = ";
				print(stream, objPtr, metadata.type);
			}
		}

		stream << "}" << ENDL;
	}

}