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

struct Buffer
{
    size_t size;
    char* data;
};

Buffer open_file(const char* name, bool binary)
{
    Buffer ret;

    std::ifstream fs = binary ? std::ifstream(name, std::ios::binary) : std::ifstream(name);
    fs.seekg(0, std::ios::end);
    ret.size = fs.tellg();
    fs.seekg(0, std::ios::beg);

    ret.data = new char[ret.size];

    memset(ret.data, 0, ret.size);

    fs.read(ret.data, ret.size);
    fs.close();

    //std::cout << ret.size << "\t" << ret.data;

    return ret;
}

int main(int argc, char** argv)
{
    Default::init();

    try
    {
    if (argc > 1)
    {
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
            Buffer file = open_file(argv[1], false);

            //lsa
            LsAsm lsasm;
            LsFunction f = lsasm.assemble(file.data, file.size);

            char* out_name = new char[len];
            strcpy(out_name, argv[1]);
            out_name[len - 1] = 'c';

            std::ofstream out(out_name, std::ios::binary);
            if (out.is_open())
            {
                out.write((const char*)&f.code[0], f.code.size());
                out.close();
            }
            else throw Exception("Cannot write to file");

            if (argc > 2)
                if (strcmp(argv[2], "/lscpp") == 0)
                {
                    LsCpp lscpp;
                    lscpp.generate(f);    //std::cout << "end1\n";
                }

            //std::cout << "end2\n";


            Runtime r;
            r.run(f);

            system("pause");
        }
        else if (strcmp(end4, _lsc) == 0)
        {
            Buffer file = open_file(argv[1], true);

            LsCpp lscpp;

            //lsc
            LsFunction f;
            for (size_t i = 0; i < file.size; i++)
                f.code.push_back(file.data[i]);

            if (argc > 2)
                if (strcmp(argv[2], "/disasm") == 0)
                {
                    LsDisasm disasm(lscpp);
                    disasm.disasm(f);
                }


            Runtime r;
            r.run(f);

            system("pause");
        }
    }
    else
    {
        //interactive mode??
    }
    }
    catch (Exception ex)
    {
        std::cout << "Exception: " << ex.text << std::endl;
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