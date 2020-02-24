// LizardScript.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "..\LizardScriptCore\Delegate.h"
//#include "..\LizardScriptCore\NonTypedStack.h"
#include "..\LizardScriptCore\VectorsTuple.h"

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

int main()
{
    VectorsTuple<int, char*> vt;
    vt.push_back(1500);

    //NonTypedStack stack;
    //stack.push(15);
    //stack.push("abcdef");
    //Dynamic a = stack.pop();
    //Dynamic b = stack.pop();

    auto f1 = nmakedel(f);
    sample s;
    auto f2 = nmakedel(&s, &sample::f);
    std::cout << f1(5) << f2(10);
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
