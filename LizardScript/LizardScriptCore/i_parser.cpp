#include "pch.h"
#include "i_parser.h"
#include "Parser.h"
#include "Logger.h"

namespace LizardScript
{
	PoolPointer<std::vector<const void*>> runParser(const LexerData& lexerData)
	{
		Parser p = Parser(lexerData);
		//logger.add("parser tokens:");
		//logger.addSeparator();
		//logger.add(std::ref(*p.parserTokens));
		//logger.addSeparator();
		return std::move(p.run());//Parser::optimize(core, parserTokens);
	}
}