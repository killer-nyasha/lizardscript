#include "stdafx.h"
#include "Logger.h"

Logger logger;

Logger::Logger()
{
	data.reserve(16 * 1024);
}

Logger::~Logger()
{

}

void Logger::toFile()
{
	std::ofstream fs("ls_log.txt");
	fs.write(&data[0], data.size());
	fs.close();
	*this = Logger();
}
