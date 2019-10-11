#include "ByteCodeGenerator.h"
#include "LizardScriptLibrary.h"

using namespace LizardScript;

SyntaxCore* Keyword2::core;

#define OPERATOR(text, predicate, opcodes) if (kwtoken == kw.text && predicate)

const std::vector<Keyword2>& ByteCodeGenerator::initKeywords()
{
	static std::vector<Keyword2> kw =
	{
		keyword2<int, int>("+", 0, 0).setOpcode(opcode::add_int_int, 0, 1),
		keyword2<int, int>("-", 0, 0).setOpcode(opcode::sub_int_int, 0, 1),
		keyword2<int, int>("*", 0, 0).setOpcode(opcode::mul_int_int, 0, 1),
		keyword2<int, int>("/", 0, 0).setOpcode(opcode::div_int_int, 0, 1),

		keyword1<int>("++", 1).setOpcode(opcode::inc, 0, 0),
		keyword1<int>("--", 1).setOpcode(opcode::dec, 0, 0),

		keyword2<bool, int, int>("<", 0, 0).setOpcode(opcode::less_int_int, 0, 1),
		keyword2<bool, int, int>(">", 0, 0).setOpcode(opcode::more_int_int, 0, 1),

		keyword2<int*, int, int>("=", 1, 0).setOpcode(opcode::set_32, 0, 1),
		keyword2<stringptr, stringptr>("=", 1, 0).setOpcode(opcode::set_stringptr, 0, 1),

		keyword2<void, void>("=", 2, 1).setOpcode((is_x64() ? opcode::set_64 : opcode::set_32), 0, 1),

		keyword2<float*, float, float>("=", 1, 0).setOpcode(opcode::set_32, 0, 1),

		keyword2<float*, float, int>("=", 1, 0).setOpcode(opcode::int_to_float, 1, 1).setOpcode(opcode::set_32, 0, 1),

		keyword2<float, int, float>("+", 0, 0).setOpcode(opcode::int_to_float, 0, 0).setOpcode(opcode::add_float_float, 0, 1),
		keyword2<float, int, float>("-", 0, 0).setOpcode(opcode::int_to_float, 0, 0).setOpcode(opcode::sub_float_float, 0, 1),
		keyword2<float, int, float>("*", 0, 0).setOpcode(opcode::int_to_float, 0, 0).setOpcode(opcode::mul_float_float, 0, 1),
		keyword2<float, int, float>("/", 0, 0).setOpcode(opcode::int_to_float, 0, 0).setOpcode(opcode::div_float_float, 0, 1),

		keyword2<float, int>("+", 0, 0).setOpcode(opcode::int_to_float, 1, 1).setOpcode(opcode::add_float_float, 0, 1),
		keyword2<float, int>("-", 0, 0).setOpcode(opcode::int_to_float, 1, 1).setOpcode(opcode::sub_float_float, 0, 1),
		keyword2<float, int>("*", 0, 0).setOpcode(opcode::int_to_float, 1, 1).setOpcode(opcode::mul_float_float, 0, 1),
		keyword2<float, int>("/", 0, 0).setOpcode(opcode::int_to_float, 1, 1).setOpcode(opcode::div_float_float, 0, 1),

		keyword2<float, float>("+", 0, 0).setOpcode(opcode::add_float_float, 0, 1),
		keyword2<float, float>("-", 0, 0).setOpcode(opcode::sub_float_float, 0, 1),
		keyword2<float, float>("*", 0, 0).setOpcode(opcode::mul_float_float, 0, 1),
		keyword2<float, float>("/", 0, 0).setOpcode(opcode::div_float_float, 0, 1),
	};

	return kw;
}
