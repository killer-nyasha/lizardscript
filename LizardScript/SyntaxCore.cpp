#include "stdafx.h"
#include "SyntaxCore.h"
#include "DefaultSyntaxCore.h"
#include "algorithms.h"

using namespace LizardScript;

void SyntaxCore::confirmChanges()
{
	quickSort(textChars);
	quickSort(breakChars);
	pQuickSort(keywords);

	for (auto& kw : keywords)
	{
		bool isOk = false;

		if (kw->listA())
		{
			isOk = true;
			keywords_listA.push_back(kw);
		}
		if (kw->listB())
		{
			isOk = true;
			keywords_listB.push_back(kw);
		}

		if (!isOk)
			throw Exception("Is's a fucking odd keyword");
	}
}

namespace LizardScript
{
	SyntaxCore defaultSyntaxCore = createDefaultSyntaxCore();
}