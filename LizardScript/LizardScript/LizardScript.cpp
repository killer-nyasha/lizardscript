// LizardScript.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>

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

int main(int argc, char** argv)
{
    Default::init();

    if (argc > 1)
    {
        size_t length;
        char* buffer;

        //где-то нужно читать бинарный файл, а где-то текстовый
        std::ifstream fs(argv[1]/*, std::ios::binary*/);
        fs.seekg(0, std::ios::end);
        length = fs.tellg();
        fs.seekg(0, std::ios::beg);

        buffer = new char[length];
        fs.read(buffer, length);
        fs.close();

        size_t len = strlen(argv[1]);
        char* end3 = argv[1] + len - 3;
        char* end4 = argv[1] + len - 4;

        const char* _ls = ".ls";
        const char* _lsa = ".lsa";
        const char* _lsc = ".lsc";

        if (strcmp(end3, _ls) == 0)
        {
            //ls

            std::cout << "not implemented\n";
            system("pause");
        }
        else if (strcmp(end4, _lsa) == 0)
        {
            //lsa
            LsCpp lscpp;

            LsAsm lsasm;
            LsFunction f = lsasm.assemble(buffer, length);

            Runtime r;
            r.run(f);

            char* out_name = new char[len];
            strcpy(out_name, argv[1]);
            out_name[len - 1] = 'c';

            std::ofstream out(out_name, std::ios::binary);

            out.write((const char*)&f.code[0], f.code.size());

            out.close();

            system("pause");
        }
        else if (strcmp(end4, _lsc) == 0)
        {
            LsCpp lscpp;

            //lsc
            LsFunction f;
            for (size_t i = 0; i < length; i++)
                f.code.push_back(buffer[i]);

            LsDisasm disasm(lscpp);
            disasm.disasm(f);

            Runtime r;
            r.run(f);

            system("pause");
        }
    }
    else
    {
        //interactive mode??
    }


    //-hello -- -(-world--world1)
    //-hello- --(-world--world1)-
    //auto ldata = runLexer(Default::syntaxCore, "[f 1](1,g(2,3))");

    //print_ldata(*ldata);

    //runParser(*ldata);

    //print_ldata(*ldata);

    //LsCpp lscpp;

    //LsDisasm disasm(lscpp);
    //disasm.disasm(f);

    //auto f1 = nmakedel(f);
    //sample s;
    //auto f2 = nmakedel(&s, &sample::f);
    //std::cout << f1(5) << f2(10);
}