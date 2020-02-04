#pragma once
#include "LizardScriptCompiler.h"

namespace LizardScript
{
	extern LizardScriptCompiler* standartCompiler;

	void initLs();

	template <typename T>
	inline TypedExpr<T> ls(const TCHAR* text)
	{
		return LizardScript::standartCompiler->create<T>(text);
	}

	inline Expr operator ""  _ls(const TCHAR * text)
	{
		return LizardScript::standartCompiler->create(TYPEINFO(void), text);
	}
}