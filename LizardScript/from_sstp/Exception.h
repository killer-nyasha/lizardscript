#pragma once
#include <string>

class Exception
{
public:
	std::string text;

	Exception() { }
	Exception(const std::string& text) : text(text) { }
	Exception(std::string&& text) : text(text) { }
	Exception(const char* text) : text(text) { }
};