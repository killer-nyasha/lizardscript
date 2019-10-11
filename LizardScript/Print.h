#pragma once
//#include "interpreter_util.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_BLUE "\033[1;36m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_NC "\033[0;1m"
#define WARNING(...) std::cout << COLOR_YELLOW << "Warning: " << __VA_ARGS__ << COLOR_NC << std::endl;

namespace LizardScript
{
	template <typename S>
	void print(S& stream, char* object, TypeInfo currentType)
	{
		auto& metatable = globalMetadataTable[currentType];

		stream << COLOR_BLUE << currentType.t.name() << COLOR_NC << " {\n";

		for (auto& metadata : metatable.members.get<FunctionInfo>())
		{
			stream << COLOR_BLUE << metadata.type.text() << " " << COLOR_NC << metadata.name << "(";
			bool needComma = false;
			for (auto& t : metadata.args)
			{
				if (!needComma)
					needComma = true;
				else stream << ", ";
				stream << COLOR_BLUE << t.text() << COLOR_NC;
			}
			stream << ");\n";
		}
		for (auto& metadata : metatable.members.get<FieldInfo>())
		{
			stream << COLOR_BLUE << metadata.type.text() << COLOR_NC << " " << metadata.name;

			if (metadata.type == makeTypeInfo<int>())
			{
				int* p = (int*)(object + metadata.offset);
				for (size_t i = 0; i < metadata.type.ptr; i++)
					p = *((int**)p);
				stream << " = " << *p << ";\n";
			}
			else if (metadata.type == makeTypeInfo<float>())
			{
				int* p = (int*)(object + metadata.offset);
				for (size_t i = 0; i < metadata.type.ptr; i++)
					p = *((int**)p);
				stream << " = " << *(float*)p << ";\n";
			}
			else
			{
				char* objPtr = *(char**)(object + metadata.offset);

				stream << " = ";
				if (objPtr == nullptr)
					stream << COLOR_BLUE << "null;\n" << COLOR_NC;
				else 
				{
					//точно не нужно??
					//for (size_t i = 0; i < metadata.type.ptr; i++)
					//	objPtr = *((char**)objPtr);
					print(stream, objPtr, metadata.type);
				}
					
			}
		}

		stream << "}\n";
	}

	template <typename S, typename T>
	void print(S& stream, T& object)
	{
		print(stream, (char*)&object, makeTypeInfo<T>());
	}

	template <typename T>
	void print_r(void* ptr)
	{
		print(std::cout, *(T*)ptr);
	}
}