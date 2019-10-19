#pragma once
#include "Expr.h"

namespace LizardScript
{
	class Runtime
	{
	public:
		Expr& expr;
		void* ths;
		TypeInfo type;
		//size_t offsetsIndex;

		void* registers[17];

		//std::tuple<int_from<int>, int_from<float>> constantsIndex;

		//NontypedStack stack;

		//!stack with int/float values
		//std::stack<double> values = std::stack<double>();

		//!stack with pointers (they are stored separately from values for type safety)
		//std::stack<void*> pointers = std::stack<void*>();

		template <typename T>
		Runtime(Expr& e, T& ths) : expr(e), ths(&ths), type(makeTypeInfo<T>()) { run(); }

		Runtime(Expr& e, TypeInfo t, void* ths) : expr(e), ths(ths), type(t) { run(); }

		void run();
	};

	template <typename T>
	void TypedExpr<T>::operator()(T& ths)
	{
		Runtime(*this, ths);
	}
}