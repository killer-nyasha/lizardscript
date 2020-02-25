// LizardScript.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "..\LizardScriptCore\Delegate.h"
//#include "..\LizardScriptCore\NonTypedStack.h"
#include "..\LizardScriptCore\VectorsTuple.h"

#include "..\LizardScriptCore\i_lexer.h"
#include "..\LizardScriptCore\LizardScriptDefault.h"

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

int main()
{
    Default::init();
    auto data = runLexer(Default::syntaxCore, "hello+world(1,2)()");

    for (size_t i = 0; i < data->tokens.size(); i++)
        std::cout << (*data)[i];

    //auto f1 = nmakedel(f);
    //sample s;
    //auto f2 = nmakedel(&s, &sample::f);
    //std::cout << f1(5) << f2(10);
}