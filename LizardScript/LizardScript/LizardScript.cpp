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

int main()
{
    Default::init();
    auto ldata = runLexer(Default::syntaxCore, "[hello+world](1,2)()");
    auto pdata = runParser(*ldata);

    //for (size_t i = 0; i < pdata->size(); i++)
    //    std::cout << (*pdata)[i] << "    ";



    //auto f1 = nmakedel(f);
    //sample s;
    //auto f2 = nmakedel(&s, &sample::f);
    //std::cout << f1(5) << f2(10);
}