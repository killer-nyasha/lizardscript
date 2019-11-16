#include "ByteCodeGenerator.h"
#include "LizardScriptLibrary.h"

using namespace LizardScript;

SyntaxCore* IOperator::core;

#define CAST/*(predicate, ...)*/ if (
#define THEN ) { 
#define ENDCAST if (from.full_eq(to)) return true; else return cast(reg, to); }

#define KEYWORD(text) if (_tcscmp(kwtoken->value, text) == 0)//временно так, потом постараюсь всё-таки вынести структуру
#define WHEN { int csize = code.data.size(); if (
#define OPCODE ) { code.push(
#define RETURNS(T) ); r1.type = T; reg.alloc(r1.type); return true; } else code.data.resize(csize); }
#define VOID ); } else code.data.resize(csize); }

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

bool ByteCodeGenerator::cast(typed_reg reg, TypeInfo to)
{
	TypeInfo& from = reg.type;

	if (from.full_eq(to))
		return true;

	CAST
		from.ptr > 0 && to.ptr == from.ptr
		&& std::find(globalMetadataTable[from].parents.begin(),
			globalMetadataTable[from].parents.end(), ParentInfo{ 0, to })
		!= globalMetadataTable[from].parents.begin()
		THEN from = to;
	ENDCAST;

	CAST
		from.ptr > to.ptr && from.byValueSize <= sizeof(void*)
		THEN open_reg(reg, to.ptr);
	ENDCAST;

	CAST
		from.ptr > to.ptr && to.ptr >= 1
		THEN open_reg(reg, to.ptr);
	ENDCAST;

	//CAST
	//	from.ptr > 0
	//	&& from.openedSize() > sizeof(void*)
	//	&& from.ptr > to.ptr
	//	THEN FieldInfo& f = newLocalVariable(from, "_temp_"); this->reg.free();
	//	code << opcode::push_stackbase << reg;
	//	code << opcode::push_offset << reg << (short int)f.offset;
	//ENDCAST;

	CAST
		from.full_eq(TYPEINFO(int))
		&& to.full_eq(TYPEINFO(float))
		THEN code << opcode::int_to_float << reg; from.t = to.t;
	ENDCAST;

	CAST
		from.full_eq(TYPEINFO(float))
		&& to.full_eq(TYPEINFO(int))
		THEN code << opcode::float_to_int << reg; from.t = to.t;
	ENDCAST;

	return false;
}

bool ByteCodeGenerator::addUnary(Keyword* kwtoken, typed_reg r1)
{
	auto& keywords = initUnary(core);
	int csize = code.data.size();
	auto pair = regindex_pair(r1, r1);

	for (auto& kw : keywords)
	{
		if (kwtoken == kw.text && cast(r1, kw.type1))
		{
			code << kw.code;
			code << pair;
			reg.alloc(r1.type);
			return true;
		}
		else code.data.resize(csize);
	}

	return false;
}

bool ByteCodeGenerator::addBinary(Keyword* kwtoken, typed_reg r1, typed_reg r2)
{
	auto& keywords = initBinary(core);
	int csize = code.data.size();
	auto pair = regindex_pair(r1, r2);

	for (auto& kw : keywords)
	{
		if (kwtoken == kw.text && cast(r1, kw.type1) && cast(r2, kw.type2))
		{
			code << kw.code;
			code << pair;
			r1.type = kw.rettype;
			reg.alloc(r1.type);
			return true;
		}
		else code.data.resize(csize);
	}

	KEYWORD("=") WHEN
	(r1.type.size() <= 4) && (r2.type.size() <= 4) &&
	cast(r1, r1.type.withPtr(2)) &&
	cast(r2, r1.type.withPtr(1))
	OPCODE opcode::set_32, pair
	RETURNS(r1.type.withPtr(2))

	KEYWORD("=") WHEN
	(r1.type.size() <= 4) && (r2.type.size() <= 4) &&
	cast(r1, r1.type.withPtr(1)) &&
	cast(r2, r1.type.withPtr(0))
	OPCODE opcode::set_32, pair
	RETURNS(r1.type.withPtr(1))

	KEYWORD("=") WHEN
	cast(r1, r1.type.withPtr(1)) &&
	cast(r2, r1.type.withPtr(1))
	OPCODE opcode::set_big, pair, (short)r1.type.byValueSize
	RETURNS(r1.type.withPtr(1))

	std::string opname = std::string("operator") + kwtoken->value;
	for (auto f : globalMetadataTable[r1.type].members.get<FunctionInfo>())
	{
		if (f.name == opname)
		{
			//reg.stackEmul.push(r1);
			//reg.stackEmul.push(r2);
			//typed_reg rths = reg.alloc(r1.type);

			//code.push(opcode::mov, regindex_pair(r1, r2));
			//code.push(opcode::mov, regindex_pair(r2, rths));
			//std::swap(r1.index, r2.index);
			//std::swap(r2.index, rths.index);

			PossibleFunctionCalls call;
			call.functions.push_back(&f);
			call.index = r1.index;

			reg.alloc(r1.type);
			reg.alloc(r2.type);
			findFunctionToCall(call);
			return true;
		}
	}

	//можно объединить в два оператора, зависящие только от размера?
	//return false;
	throw Exception(std::string("Unknown operator \"") + kwtoken->value + "\" for types " + r1.type.text() + " and " + r2.type.text() + ".");
}

const std::vector<UnaryOperator>& ByteCodeGenerator::initUnary(SyntaxCore& core)
{
	UnaryOperator::core = &core;
	static std::vector<UnaryOperator> kw =
	{
		UnaryOperator("++", TYPEINFO(int, 1), opcode::inc),
		UnaryOperator("--", TYPEINFO(int, 1), opcode::dec),
	};

	return kw;
}

const std::vector<BinaryOperator>& ByteCodeGenerator::initBinary(SyntaxCore& core)
{
	BinaryOperator::core = &core;
	static std::vector<BinaryOperator> kw =
	{
		BinaryOperator("+", TYPEINFO(int), TYPEINFO(int), opcode::add_int_int),
		BinaryOperator("-", TYPEINFO(int), TYPEINFO(int), opcode::sub_int_int),
		BinaryOperator("*", TYPEINFO(int), TYPEINFO(int), opcode::mul_int_int),
		BinaryOperator("/", TYPEINFO(int), TYPEINFO(int), opcode::div_int_int),

		BinaryOperator("+", TYPEINFO(float), TYPEINFO(float), opcode::add_float_float),
		BinaryOperator("-", TYPEINFO(float), TYPEINFO(float), opcode::sub_float_float),
		BinaryOperator("*", TYPEINFO(float), TYPEINFO(float), opcode::mul_float_float),
		BinaryOperator("/", TYPEINFO(float), TYPEINFO(float), opcode::div_float_float),

		BinaryOperator(">", TYPEINFO(int), TYPEINFO(int), opcode::more_int_int, TYPEINFO(bool)),
		BinaryOperator("<", TYPEINFO(int), TYPEINFO(int), opcode::less_int_int, TYPEINFO(bool)),

		BinaryOperator("=", TYPEINFO(stringptr, 1), TYPEINFO(stringptr), opcode::set_stringptr),
	};

	return kw;
}
