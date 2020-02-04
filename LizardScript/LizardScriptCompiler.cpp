#include "stdafx.h"

#include "LizardScriptCompiler.h"
#include "ILexer.h"
#include "IParser.h"
#include "LsExpr.h"
#include "IByteCodeGenerator.h"
#include "LizardScriptLibrary.h"

#include "Pools.h"

using namespace LizardScript;

namespace LizardScript
{
	std::map<TypeInfo, TypeInfoEx> globalMetadataTable;
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

	PoolPointer<LexerData> ld = runLexer(core, t);

	logger.add("lexer tokens:");
	logger.addSeparator();
	logger.add(ld->tokens);
	logger.addSeparator();

	std::vector<TCHAR*>* pt = runParser(ld->tokens);
	expr = runByteCodeCompiler(type, *pt);
	delete pt;

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