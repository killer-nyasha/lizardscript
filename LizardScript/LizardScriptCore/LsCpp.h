#pragma once
#include <map>
#include <fstream>
#include <iostream>

#include "OpcodesText.h"
#include "LsFunction.h"
#include "StringBuilder.h"
#include "Exception.h"

class LsCpp;

inline void stringReplace(std::string& input, const std::string& ext1, const std::string& ext2)
{
	while (true)
	{
		auto iter1 = input.find(ext1, 0);

		if (iter1 == std::string::npos)
			break;

		auto iter2 = iter1 + ext1.length();
		input.replace(iter1, iter2 - iter1, ext2);

	}
}

inline void dirFromFile(std::string& input)
{
	auto iter1 = input.find_last_of(
#ifdef _WIN32
		'\\'
#else
		'/'
#endif
		, input.length());

	input.replace(iter1 + 1, input.length(), "");
}

struct LsCppCompilerCall
{
	std::string path_to_lizard_script_core_lib;
	std::string path_to_lizard_script_core_src;
	std::string compiler_call_string; //#OUR_FILE - macro
	std::string executable_extension;

	void read(std::string path_to_exe)
	{
		dirFromFile(path_to_exe);

		path_to_exe += "LsCppSettings.txt";

		std::ifstream fs(path_to_exe);

		if (!fs.is_open())
			throw Exception("Cannot read LsCpp setting file");

		std::getline(fs, path_to_lizard_script_core_lib);
		std::getline(fs, path_to_lizard_script_core_src);
		std::getline(fs, compiler_call_string);
		std::getline(fs, executable_extension);
	}

	void call(const std::string& input_path)
	{
		std::string new_compiler_call_string = compiler_call_string;

		std::string input_dir = input_path;
		dirFromFile(input_dir);

		if (input_dir.empty())
			throw Exception("LsCpp needs full path as an argument to call a C++ compiler");

		stringReplace(new_compiler_call_string, "$INPUT_FILE", input_path);
		stringReplace(new_compiler_call_string, "$INPUT_DIR", input_dir);
		stringReplace(new_compiler_call_string, "$LS_CORE_LIB_DIR", path_to_lizard_script_core_lib);
		stringReplace(new_compiler_call_string, "$LS_CORE_SRC_DIR", path_to_lizard_script_core_src);

		std::cout << new_compiler_call_string.c_str();
		system((std::string("\"") + new_compiler_call_string + "\"").c_str());

		std::string output_path = input_path;
		stringReplace(output_path, ".cxx", executable_extension);

		system(output_path.c_str());
	}
};

template <typename T>
struct FROM_BINARY
{
	template <typename... A>
	static T get(A... args)
	{
		T ret;
		size_t i = 0;
		FORVARIADIC(((unsigned char*)&ret)[i++] = args);
		return ret;
	}
};

struct AbstractLsCppOpcode
{
	LsCode opcode;
	virtual void print(LsCpp& lscpp) = 0;
	size_t codeget_size = 0;	
	virtual void inc_codeget_size() = 0;
};

struct LsCppSpec
{
	virtual void dummy() { }
	//virtual void append(LsCpp& lscpp) = 0;
};

struct LsCppVariable
{
	const char* type;
	//const char* name;
	//std::vector<unsigned char> data;

	unsigned char data[32];
	size_t data_size = 0;
};

using OFFSET_T = unsigned char;

class LsCpp
{
private:

	std::vector<AbstractLsCppOpcode*> opcodes;

public:

	std::map<const char*, LsCppVariable, cmp_str> variables;

	AbstractLsCppOpcode* opcodes_table[256];
	
	//base class?
	const LsFunction* f;
	size_t eip = 0;
	StringBuilder text;

	std::string generate(const LsFunction& f);

	LsCpp();
};