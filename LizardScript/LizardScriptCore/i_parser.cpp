#include "pch.h"
#include "i_parser.h"
#include "Parser.h"
#include "Logger.h"

namespace LizardScript
{
	void runParser(LexerData& lexerData)
	{
		Parser p = Parser(lexerData);
		//logger.add("parser tokens:");
		//logger.addSeparator();
		//logger.add(std::ref(*p.parserTokens));
		//logger.addSeparator();

		auto tokens = p.run();
		//LexerData ret = LexerData();
		lexerData.tokens = std::move(tokens);

		//return ret;//Parser::optimize(core, parserTokens);
	}
}