#pragma once
#include "../LizardScript/Expr.h"

namespace LizardScript
{
	void dummy();

	class Lightspeed
	{
		char* dcode;
		char *asmBuffer, *asmBufferPtr;
		size_t dcodeSize;
		size_t asmBufferSize;

		char* pNasmDll;
		char* nasmDump;

		bool protectionWasSet;

		template <typename T1, typename T2, typename... T>
		void line(T1 l1, T2 l2, T... l)
		{
			elem(l1);
			elem(l2);
			int dummy[] = 
			{ (elem(l), 0)... };
			*(asmBufferPtr++) = '\0';
		}

		template <typename T1, typename T2>
		void line(T1 l1, T2 l2)
		{
			elem(l1);
			elem(l2);
			*(asmBufferPtr++) = '\0';
		}

		template <typename T1>
		void line(T1 l1)
		{
			elem(l1);
			*(asmBufferPtr++) = '\0';
		}

		void elem(const char* l)
		{
			while (*l != '\0')
				*(asmBufferPtr++) = *(l++);
		}

		void elem(int i)
		{
			char buffer[24];
			_itoa(i, buffer, 10);
			elem(buffer);
		}

		void elem(size_t i)
		{
			char buffer[24];
			_itoa((int)i, buffer, 10);
			elem(buffer);
		}

	public:

		Lightspeed(size_t dcodeSize, size_t asmBufferSize);
		Lightspeed(const Lightspeed&) = delete;
		~Lightspeed();

		void operator<<(Expr& expr);

		void assemble();

		void call()
		{
			if (!protectionWasSet)
				setProtection();

			void(*pDummy)() = &dummy;
			char buf[sizeof(pDummy)];
			*reinterpret_cast<void(**)()>(&buf) = &dummy;
			*reinterpret_cast<void**>(&buf) = dcode;
			(*reinterpret_cast<void(**)()>(&buf))();
		}

		void setProtection(bool isExecutable = true);

		//void set();
	};
}