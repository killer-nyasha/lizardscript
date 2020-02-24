/*!
\file Logger.h
\brief derived from StringBuilder. it allows write its content to file.
There is a standart Logger instanse which is implemented in Logger.cpp
\author killer-nyasha
\version 0.2
\date 23.00.2020
*/
#pragma once
#include <string>
#include <vector>

#include "StringBuilder.h"

class Logger : public StringBuilder
{
private:
	
	std::string filename;

	void init();

public:

	Logger(const std::string& filename);

	//!calls toFile. so Logger must be deleted correctly to guarantee that all logs were written
	~Logger();

	//!write all records to file and delete them from memory
	void toFile();
};

extern Logger logger;