#include "stdafx.h"
#include "IParser.h"
#include "Parser.h"

namespace LizardScript
{
	PoolPointer<std::vector<TCHAR*>> runParser(std::vector<TCHAR*>& r)
	{
		Parser p = Parser(r);

		logger.add("parser tokens:");
		logger.addSeparator();
		logger.add(std::ref(**p.parserTokens));
		logger.addSeparator();
		return std::move(p.parserTokens);//Parser::optimize(core, parserTokens);
	}
}