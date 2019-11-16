#define _CRT_SECURE_NO_WARNINGS

#include <chrono>

#include "LizardScriptCompiler.h"
#include "Lexer.h"
#include "Parser.h"
#include "Expr.h"
#include "ByteCodeGenerator.h"
#include "LizardScriptLibrary.h"

using namespace LizardScript;

std::vector<TCHAR> global_values;
std::vector<TCHAR*> global_tokens;
std::vector<TCHAR*> global_parserTokens; 
std::stack<Keyword*> global_parserStack;

namespace LizardScript
{
	std::map<TypeInfo, TypeInfoEx> globalMetadataTable;
	LizardScriptCompiler* standartCompiler;
}

void LizardScriptCompiler::runLexer(const TCHAR* t, std::vector<TCHAR>& values, std::vector<TCHAR*>& tokens)
{
	Lexer(core, t, values, tokens);
}

std::vector<TCHAR*> LizardScriptCompiler::runParser(std::vector<TCHAR*>& r)
{
	global_parserTokens.resize(0);
	Parser(core, r, global_parserTokens, global_parserStack);
	return global_parserTokens;//Parser::optimize(core, parserTokens);
}

Expr LizardScriptCompiler::runByteCodeCompiler(TypeInfo type, std::vector<TCHAR*>& tokens)
{
	return ByteCodeGenerator(tokens, type, core).e;
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

	size_t nCapacity = _tcslen(t) + 256;
	if (global_values.capacity() < nCapacity)
		global_values.reserve(nCapacity);

	global_values.resize(0);
	global_tokens.resize(0);

	runLexer(t, global_values, global_tokens);

	logger.add("lexer tokens:");
	logger.addSeparator();
	logger.add(global_tokens);
	logger.addSeparator();

	runParser(global_tokens);

	logger.add("parser tokens:");
	logger.addSeparator();
	logger.add(global_parserTokens);
	logger.addSeparator();

	expr = runByteCodeCompiler(type, global_parserTokens);

	logger.addSeparator();
	logger.add("disasm:");
	logger.addSeparator();
	expr.disasm();
	logger.addSeparator();

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
		logger.toFile();
	}
	catch (...)
	{
		logger.add("Unknown compilation error");
		std::cout << logger.data;
		logger.toFile();
	}
	else
	{
		create_impl(type, t);
		logger.toFile();
	}

	//expr.optimize();
	return std::move(expr);
}