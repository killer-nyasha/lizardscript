/*!
\file Print.h
\brief prints all fields of some object dynamically, using metadata
\author killer-nyasha
\version 0.2
\date 23.00.2020
*/
#pragma once
#include <iostream>

#include "TypeInfo.h"

namespace LizardScript
{
	//!print all fields of some object
	//!\param object Non-typed pointer to object
	//!\param currentType Metadata of its type
	void print(std::ostream& stream, const char* object, const TypeInfo& currentType);

	//!template wrapper, which gets TypeInfo automatically
	template <typename T>
	void print(std::ostream& stream, const T& object)
	{
		print(stream, (const char*)&object, typeInfo<T>());
	}
}