// InterpreterInteractive.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <typeindex>
#include <string>
#include <fstream>
#include <streambuf>

#include "../LizardScript/LizardScriptCompiler.h"
#include "../LizardScript/Print.h"
#include "../LizardScript/Runtime.h"
#include "../LizardScript/LizardScriptLibrary.h"

using namespace LizardScript;

struct B
{
	int x = 1;
	float f = 1;
	B* b = nullptr;

	B() { }

	B operator+(B& b)
	{
		B ret;
		ret.x = x + b.x;
		ret.f = f + b.f;
		return ret;
	}
};

struct AParent1
{
	virtual int test() { return 0; }
};

struct AParent2
{
	virtual int a() { return 0; }
};

struct A : public AParent1, AParent2
{
	int i = 10;
	float f = 20.0f;
	int k = 30;

	A* a = nullptr;
	B* b = nullptr;

	B inlb;

	virtual int test()
	{
		return f * 2;
	}

	void test2(int a, int b)
	{
		f = a + b;
	}

	A()
	{
		i = 100;
	}
};

void run(A& t, std::string& source)
{
	TypedExpr<A> e = script<A>(&source[0]);
	Runtime(e, t);
}

int main(int argc, char** argv)
{
	if (lsl.printHello)
	{
		std::cout << COLOR_GREEN
			<< "**********************************************************************\n"
			<< "** LizardScript v0.1 Interactive\n"
			<< "** 11.10.2019, killer-nyasha\n"
			<< "**********************************************************************\n" << COLOR_NC;
	}

	standartCompiler = new LizardScriptCompiler(defaultSyntaxCore);
	LizardScriptLibrary::init_ls_standart_meta();

	METAGEN_CLASS(A) WITH_MEMBERS(, FIELD(i), FIELD(f), FIELD(k), FIELD(a), FIELD(b), FIELD(inlb),
		PARAMS()::FUNC(CtorProvider<A>) WITHNAME(ctor),
		PARAMS()::FUNC(A) WITHNAME(test),
		PARAMS(int, int)::FUNC(A) WITHNAME(test2)
	);
	METAGEN_CLASS(B) WITH_MEMBERS(, FIELD(x), FIELD(f), FIELD(b),
		PARAMS()::FUNC(CtorProvider<B>) WITHNAME(ctor),
		PARAMS(B&)::FUNC(B) WITHNAME(operator+)
	);

	endMetadata();

	A t;

	if (argc > 1)
	{
		std::ifstream s(argv[1]);
		std::string source((std::istreambuf_iterator<char>(s)), std::istreambuf_iterator<char>());		
		
		if (lsl.printState)
			LizardScript::print(std::cout, t);

		run(t, source);

		if (lsl.printState)
			LizardScript::print(std::cout, t);
	}
	else while (true)
	{
		if (lsl.printState)
			LizardScript::print(std::cout, t);

		std::cout << COLOR_GREEN << ">     ";

		std::string input;
		std::getline(std::cin, input);

		std::cout << COLOR_NC;

		run(t, input);
	}
	system("pause");
}