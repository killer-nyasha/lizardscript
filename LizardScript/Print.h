#pragma once
#include "interpreter_util.h"
//#include "metagen.h"

#ifdef COLOR_TEXT
#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_BLUE "\033[1;36m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_NC "\033[0;1m"
#else
#define COLOR_RED ""
#define COLOR_GREEN ""
#define COLOR_BLUE ""
#define COLOR_YELLOW ""
#define COLOR_NC ""
#endif
#define WARNING(...) std::cout << COLOR_YELLOW << "Warning: " << __VA_ARGS__ << COLOR_NC << std::endl;

#include <iostream>

namespace LizardScript
{
	void print(std::ostream& stream, char* object, TypeInfo currentType);

	template <typename T>
	void print(std::ostream& stream, T& object)
	{
		print(stream, (char*)&object, makeTypeInfo<T>());
	}
}