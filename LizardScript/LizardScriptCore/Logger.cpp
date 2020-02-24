#include "pch.h"
#include <fstream>

#include "Logger.h"

Logger logger("ls_log.txt");

Logger::Logger(const std::string& filename) : filename(filename)
{
	data.reserve(16 * 1024);
	init();
}

void Logger::init()
{
	data.clear();
}

Logger::~Logger()
{
	toFile();
}

void Logger::toFile()
{
	std::ofstream fs(filename);
	fs.write(&data[0], data.size());
	fs.close();
	init();
}
