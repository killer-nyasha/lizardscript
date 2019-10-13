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

#include "../LsLightspeed/Lightspeed.h"

using namespace LizardScript;

struct B
{
	int x = 1;
	float f = 1;
	B* b = nullptr;

	B() { }
};

struct A
{
	int i = 10;
	float f = 20.0f;
	int k = 30;

	A* a = nullptr;
	B* b = nullptr;

	void test()
	{
		f *= 2;
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

Lightspeed* lightSpeed = new Lightspeed(4096, 4096);

void run(A& t, std::string& source)
{
	TypedExpr<A> e;
	try
	{
		e = script<A>(&source[0]);
		try
		{
			if (lightSpeed != nullptr)
			{
				*lightSpeed << e;
				lightSpeed->assemble();
				lightSpeed->call();
			}
			else Runtime(e, t);
		}
		catch (Exception ex)
		{
			std::cout << COLOR_RED << "Runtime error: \"" << ex.text << "\"" << COLOR_NC << std::endl;
		}
	}
	catch (Exception ex)
	{
		std::cout << COLOR_RED << "Syntax error: \"" << ex.text << "\"" << COLOR_NC << std::endl;
		//e.disasm();
	}
}

int main(int argc, char** argv)
{

	//lightspeed();

	if (global_lsl.printHello)
	{
		std::cout << COLOR_GREEN
			<< "**********************************************************************\n"
			<< "** LizardScript v0.1 Interactive\n"
			<< "** 11.10.2019, killer-nyasha\n"
			<< "**********************************************************************\n" << COLOR_NC;
	}

	standartCompiler = new LizardScriptCompiler(defaultSyntaxCore);
	LizardScriptLibrary::init_ls_standart_meta();

	METAGEN(A, FIELD(i), FIELD(f), FIELD(k), FIELD(a), FIELD(b), 
		PARAMS()::FUNC(CtorProvider<A>, ctor), 
		PARAMS()::FUNC(A, test), 
		PARAMS(int, int)::FUNC(A, test2));
	METAGEN(B, FIELD(x), FIELD(f), FIELD(b),
		PARAMS()::FUNC(CtorProvider<B>, ctor));

	A t;

	if (argc > 1)
	{
		std::ifstream s(argv[1]);
		std::string source((std::istreambuf_iterator<char>(s)), std::istreambuf_iterator<char>());		
		
		if (global_lsl.printState)
			LizardScript::print(std::cout, t);

		run(t, source);

		if (global_lsl.printState)
			LizardScript::print(std::cout, t);
	}
	else while (true)
	{
		if (global_lsl.printState)
			LizardScript::print(std::cout, t);

		std::cout << COLOR_GREEN << ">     ";

		std::string input;
		std::getline(std::cin, input);

		std::cout << COLOR_NC;

		run(t, input);
	}
	system("pause");
}