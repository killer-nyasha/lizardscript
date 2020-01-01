#include "stdafx.h"
#include "SyntaxCore.h"
#include "DefaultSyntaxCore.h"
#include "algorithms.h"

using namespace LizardScript;

void SyntaxCore::sort()
{
	QuickSort(textChars);
	QuickSort(breakChars);
	QuickSort(keywords);
}

namespace LizardScript
{
	SyntaxCore defaultSyntaxCore = createDefaultSyntaxCore();
}