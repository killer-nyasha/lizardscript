#include "stdafx.h"
#include "CppUnitTest.h"

#include "../LizardScript/LizardScriptCompiler.h"
#include "../LizardScript/Print.h"
#include "../LizardScript/Runtime.h"

using namespace LizardScript;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

struct Test01
{
	int i = 10;
	float j = 20.0f;
	int k = 30;
};

namespace InterpreterTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:

		UnitTest1()
		{
			standartCompiler = new LizardScriptCompiler(defaultSyntaxCore);
			METAGEN_CLASS(Test01) WITH_MEMBERS(, FIELD(i), FIELD(j), FIELD(k));
		}

		TEST_METHOD(TestMethod1)
		{
			auto a = script<Test01>("i = i + k; ");
			Test01 t;
			Runtime(a, t);
			Assert::IsTrue(t.i == 40);
		}

		TEST_METHOD(TestMethod2)
		{
			auto a = script<Test01>("i = i + k; ");
			Test01 t;
			Runtime(a, t);
			Assert::IsTrue(t.i == 40);
		}

	};
}