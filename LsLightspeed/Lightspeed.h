#pragma once
#include "../LizardScript/Expr.h"

namespace LizardScript
{
	void dummy();

	class Lightspeed
	{
		char* dcode;
		char* asmBuffer;
		size_t dcodeSize;
		size_t asmBufferSize;

		char* pNasmDll;
		char* nasmDump;

		bool protectionWasSet;

	public:

		Lightspeed(size_t dcodeSize, size_t asmBufferSize);
		Lightspeed(const Lightspeed&) = delete;
		~Lightspeed();

		void operator<<(Expr& e);

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