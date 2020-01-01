#include "stdafx.h"

#include "LizardScriptCompiler.h"
#include "ILexer.h"
#include "Parser.h"
#include "Expr.h"
#include "ByteCodeGenerator.h"
#include "LizardScriptLibrary.h"

using namespace LizardScript;

std::vector<TCHAR*> global_parserTokens; 
std::stack<Keyword*> global_parserStack;

namespace LizardScript
{
	std::map<TypeInfo, TypeInfoEx> globalMetadataTable;
	LizardScriptCompiler* standartCompiler;
}

std::vector<TCHAR*> LizardScriptCompiler::runParser(std::vector<TCHAR*>& r)
{
	global_parserTokens.resize(0);
	Parser(core, r, global_parserTokens, global_parserStack);
	return global_parserTokens;//Parser::optimize(core, parserTokens);
}

Expr LizardScriptCompiler::runByteCodeCompiler(TypeInfo type, std::vector<TCHAR*>& tokens)
{
	return ByteCodeGenerator(tokens, type, core, optimized).e;
}

void LizardScriptCompiler::create_impl(TypeInfo type, const TCHAR* t)
{
	expr = Expr();

	logger.add("source code:");
	logger.addSeparator();
	logger.add(t);
	logger.addSeparator();

	std::chrono::high_resolution_clock::time_point t1 =
		std::chrono::high_resolution_clock::now();

	LexerData* ld = runLexer(core, t, false);

	logger.add("lexer tokens:");
	logger.addSeparator();
	logger.add(ld->tokens);
	logger.addSeparator();

	runParser(ld->tokens);

	logger.add("parser tokens:");
	logger.addSeparator();
	logger.add(global_parserTokens);
	logger.addSeparator();

	expr = runByteCodeCompiler(type, global_parserTokens);

	std::chrono::high_resolution_clock::time_point t2 =
		std::chrono::high_resolution_clock::now();
	if (lsl.printCompilationTime)
		std::cout << COLOR_YELLOW << "compilation time: " << (t2 - t1).count() / 1000000.0 << " ms" << COLOR_NC << std::endl;
}

Expr LizardScriptCompiler::create(TypeInfo type, const TCHAR* t, bool catchEx/*, bool catchWin32*/)
{
	if (catchEx)
	try
	{
		create_impl(type, t);
	}
	catch (Exception& ex)
	{
		logger.add("Compilation error: \"", ex.text, "\"");
		std::cout << logger.data;
	}
	catch (...)
	{
		logger.add("Unknown compilation error");
		std::cout << logger.data;
		//logger.toFile();
	}
	else
	{
		create_impl(type, t);
		//logger.toFile();
	}

	logger.addSeparator();
	logger.add("disasm:");
	logger.addSeparator();
	expr.disasm();
	logger.addSeparator();
	logger.toFile();

	//expr.optimize();
	return std::move(expr);
}