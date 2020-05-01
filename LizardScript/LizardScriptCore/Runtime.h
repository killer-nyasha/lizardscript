#pragma once
#include <vector>

namespace LizardScript
{
	using LsCode = unsigned char;

	class LsFunction
	{
	public:
		std::vector<unsigned char> code;
	};

	class Runtime
	{
	public:

		unsigned char stack[1024 * 10];

		void run(const LsFunction& f)
	};
}