// LizardScript.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "../LizardScriptCore/LizardScript.h"

int f(int x)
{
    return x * x;
}

struct sample
{
    virtual int f(int x) const
    {
        return x * x;
    }
};

using namespace LizardScript;

void print_ldata(LexerData& ldata)
{
    for (size_t i = 0; i < ldata.tokens->size(); i++)
    {
        KeywordToken* kw;
        if (ldata.tryGetKeyword(i, kw))
            std::cout << kwtypes_str(kw->type) << "";

        std::cout << ldata.text_at(i) << "    ";
    }
    std::cout << "\n\n";
}

int main()
{
    Default::init();
    //-hello -- -(-world--world1)
    //-hello- --(-world--world1)-
    auto ldata = runLexer(Default::syntaxCore, "[f 1](1,g(2,3))");

    //for (auto& k : Default::syntaxCore.keywords)
    //{
    //    std::cout << (int)k->value[0] << k->value << "\n";
    //}

    print_ldata(*ldata);

    runParser(*ldata);

    print_ldata(*ldata);

    LsCpp lscpp;

    LsFunction f;
    f.code.push_back(1);
    f.code.push_back(0);
    f.code.push_back(0);
    f.code.push_back(2);
    f.code.push_back(1);
    f.code.push_back(1);

    lscpp.generate(f);

    LsDisasm disasm(lscpp);
    disasm.disasm(f);

    //auto f1 = nmakedel(f);
    //sample s;
    //auto f2 = nmakedel(&s, &sample::f);
    //std::cout << f1(5) << f2(10);
}