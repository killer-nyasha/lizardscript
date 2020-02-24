/*!
\file Exception.h
\brief alternative to std::exception
\author killer-nyasha
\version 0.2
\date 23.00.2020
*/
#pragma once
#include <string>

#include "StringBuilder.h"

//!alternative to std::exception
//!it's easier to create instanse with description
class Exception
{
public:
	std::string text;

	Exception() { }
	Exception(const std::string& text) : text(text) { }
	Exception(std::string&& text) : text(text) { }
	Exception(const char* text) : text(text) { }

	template <typename... T>
	Exception(const T&... args)
	{
		StringBuilder sb;
		sb.add(args...);
		text = std::move(sb.data);
	}
};