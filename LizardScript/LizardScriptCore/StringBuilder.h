/*!
\file StringBuilder.h
\brief build string using variadic function of its elements
\author killer-nyasha
\version 0.2
\date 23.00.2020
*/
#pragma once
#include <string>
#include <vector>
#include <type_traits>

#include "ConsoleColor.h"

class StringBuilder
{
public:
	std::string data;

	StringBuilder& operator<<(const char* text)
	{
		data += text;
		return *this;
	}

	StringBuilder& operator<<(char* text)
	{
		data += text;
		return *this;
	}

	StringBuilder& operator<<(const std::string& text)
	{
		data += text;
		return *this;
	}

	template <typename T>
	StringBuilder& operator<<(const T& elem)
	{
		data += std::to_string(elem);
		return *this;
	}

	template <typename T>
	StringBuilder& operator<<(const std::vector<T>& vect)
	{
		for (auto& elem : vect)
			*this << "{" << elem << "}; ";
		return *this;
	}

	template <typename... T>
	void add(const T&... elems)
	{
		int i[] = { (operator<<(elems), 0)... };
		data += ENDL;
	}

	void addSeparator()
	{
		data += "******************************";
		data += ENDL;
	}
};