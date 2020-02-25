#include "pch.h"
#include "SyntaxCore.h"
//#include "DefaultSyntaxCore.h"
#include "algorithms.h"
#include "Exception.h"

using namespace LizardScript;

void SyntaxCore::confirmChanges()
{
	quickSort(textChars);
	quickSort(breakChars);
	//quickSort(keywords);

	pQuickSort(keywords);

	keywords_listA.clear();
	keywords_listB.clear();

	for (auto& kw : keywords)
	{
		bool isOk = false;

		if (kw->listA())
		{
			isOk = true;
			keywords_listA.push_back(&*kw);
		}
		if (kw->listB())
		{
			isOk = true;
			keywords_listB.push_back(&*kw);
		}

		if (!isOk)
			throw Exception("Is's a fucking odd keyword");
	}
}

//namespace LizardScript
//{
//	SyntaxCore defaultSyntaxCore = createDefaultSyntaxCore();
//}