#pragma once
#include <string>
#include <vector>

#define ENDL "\n"

//template <typename S = std::string>
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
	StringBuilder& operator<<(T elem)
	{
		using namespace std;
		data += to_string(elem);
		return *this;
	}

	template <typename T>
	StringBuilder& operator<<(std::vector<T>& vect)
	{
		for (auto& elem : vect)
			*this << "{" << elem << "}; ";
		return *this;
	}

	template <typename... T>
	void add(T... elems)
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

class Logger : public StringBuilder
{
public:
	Logger();
	~Logger();

	void toFile();

};

extern Logger logger;