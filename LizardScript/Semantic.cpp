#include "ByteCodeGenerator.h"
#include "LizardScriptLibrary.h"

using namespace LizardScript;

SyntaxCore* Keyword2::core;

#define CAST/*(predicate, ...)*/ if (
#define THEN ) { 
#define ENDCAST if (from.full_eq(to)) return true; else cast(reg, to); }

#define KEYWORD(text) if (kwtoken->value == text)
#define WHEN { int csize = code.data.size(); if (
#define OPCODE ) { code.push(
#define RETURNS(T) ); r1.type = T; reg.push(r1); } else code.data.resize(csize); }
#define VOID ); } else code.data.resize(csize); }

bool ByteCodeGenerator::cast(typed_reg reg, TypeInfo to)
{
	TypeInfo& from = reg.type;

	CAST
		from == to
		THEN from = to;
	ENDCAST;

	CAST
		from.size() <= 8
		&& to.size() <= 8
		&& from.ptr > to.ptr
		THEN open_reg(reg, to.ptr); 
	ENDCAST;

	CAST 
		from.ptr > to.ptr
		&& to.ptr >= 1
		THEN open_reg(reg, to.ptr);
	ENDCAST;

	CAST
		from == TYPEINFO(int)
		&& to == TYPEINFO(float)
		THEN code << opcode::int_to_float << reg; from.t = to.t;
	ENDCAST;

	CAST
		from == TYPEINFO(float)
		&& to == TYPEINFO(int)
		THEN code << opcode::float_to_int << reg; from.t = to.t;
	ENDCAST;

	return false;
}

void ByteCodeGenerator::addKeyword(Keyword* kwtoken, typed_reg r1, typed_reg r2)
{
	KEYWORD("+") WHEN
		cast(r1, TYPEINFO(int, 1)) &&
		cast(r2, TYPEINFO(int, 0))
		OPCODE opcode::add_int_int
		RETURNS(TYPEINFO(int))

		KEYWORD("-") WHEN
		cast(r1, TYPEINFO(int, 1)) &&
		cast(r2, TYPEINFO(int, 0))
		OPCODE opcode::sub_int_int
		RETURNS(TYPEINFO(int))

		KEYWORD("*") WHEN
		cast(r1, TYPEINFO(int, 1)) &&
		cast(r2, TYPEINFO(int, 0))
		OPCODE opcode::mul_int_int
		RETURNS(TYPEINFO(int))

		KEYWORD("/") WHEN
		cast(r1, TYPEINFO(int, 1)) &&
		cast(r2, TYPEINFO(int, 0))
		OPCODE opcode::div_int_int
		RETURNS(TYPEINFO(int))
}

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
		keyword2<void, void>("=", 1, 0).setOpcode((is_x64() ? opcode::set_64 : opcode::set_32), 0, 1),

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
