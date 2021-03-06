# -*- coding: utf-8 -*-
"""
Created on Fri Sep 13 16:59:38 2019
@author: lexa
"""
from pathlib import Path
import os
import sys
import regex
from lines_counter_beta import getIgnore, in_directory

# эти названия можно сменить на нужные тебе
endswith = ['.h', '.cpp']
out_file = 'meta.hxx'
lsclass = r'lsclass'
# -----------------------------------------
currentClass = ''
bracketCounter = 0

regUnnecessaryPatterns = regex.compile(r'public\s*:?|private\s*:?|protected\s*:?|\n|/\*.*?\*/|#define.*?\n|//.*?\n|#include.*?\n|#line.*?\n|#pragma.*?\n'
r'|#if.*?#endif|#error.*?\n|#undef.*?\n',regex.DOTALL)
regFieldParams = regex.compile(r'(?<!=\s*[^{};]+)\b\w+(?=(?![\w\s*,]+>) *(?:,|;|=|{|\())')
regMethods = regex.compile(r'operator\(\s*\)|operator[^{};\w(]*(?=\(|\w+)|\b[^{}\s;>]+(?=\s*\()')
regParamSpliter = regex.compile(r'(const[*&])?\s*[\w\s<>:()_,.&*]+(?=\s+[\w]+)')
regMethodParams = regex.compile(r'(?<=\()[^{};]*(?=\))')
regIsVirtual = regex.compile(r'\bvirtual\b')
regIsMethod = regex.compile(r'^[^{};=]+\([^{};]*\)')
regIsField = regex.compile(r'[^{};]+(?<![)\s])*;')
regInheritedClasses = regex.compile(r'(?<!=\s*[^{};]+)\b[^{};]+(?=(?![\w\s*,]+>)')
regClassName = regex.compile(r'\b\w+\b')
regIsMacro = regex.compile(r'(>\s*\w+\s*\w+\s*)$')
regBracketOrFieldOrMethod = regex.compile(r'{|}|[^{};]+(?<![)\s])*;|(?<=const)?\b(operator\b)?[^{};]*\([^{};]*\)')

def bracketComparison(s):
    if s == '(' or s == '[' or s == '<':
        return 1
    elif s == ')' or s == ']' or s == '>':
        return -1
    else:
        return 0

def removeDefaultParams(string):
    bracketsCounter = 0
    ind = string.index('(') + 1
    for i in range(ind - 1, len(string)):
        if string[i] == '(':
            bracketsCounter += 1
        elif string[i] == ')':
            bracketsCounter -= 1
        if bracketsCounter == 0:
            return string[:i + 1]

def getParamTypes(string):
    bracketsCounter = 0
    commasPositions = []
    for count, s in enumerate(string, 0):
        bracketsCounter += bracketComparison(s)
        if (s == ',' and bracketsCounter == 0):
            commasPositions.append(count)
    commasPositions.append(len(string))
    types = []
    try:
        if len(commasPositions) < 2:
            types.append(regex.search(regParamSpliter, string)[0])
        else:
            for count, c in enumerate(commasPositions, 0):
                if count != 0:
                    str_ = string[commasPositions[count - 1] + 1:commasPositions[count]].lstrip(' ')
                else:
                    str_ = string[0:commasPositions[count]].lstrip(' ')
                types.append(regex.search(regParamSpliter, str_)[0])
    except:
        return ['err']
    return types

def removeComments(string):
    string = regex.sub(regUnnecessaryPatterns, "", string)
    return string

def parseField(string, out, backSlash):
    fieldNames = regex.findall(regFieldParams, string)
    for f in fieldNames:
        out.write(',FIELD(' + f + ')' + backSlash + '\n')

def parseMethod(string, out, backSlash, virtual=''):
    try:
        funcName = regex.search(regMethods, string)[0]
    except:
        parseField(string, out, backSlash)
        return
    gluedParameters = ''
    if funcName == currentClass:
        string = removeDefaultParams(string)
    params = regex.search(regMethodParams, string)[0]
    splitedParams = getParamTypes(params) if len(params) else []
    if len(splitedParams) and splitedParams[0] != 'err':
        for count, p in enumerate(splitedParams, 0):
            if count != 0:
                gluedParameters += (',' + p)
            else:
                gluedParameters += (p)
    out.write(',PARAMS(' + gluedParameters)
    if funcName == currentClass and string.find('~') != -1:
        out.write(')::' + virtual + 'FUNC(DtorProvider<' + currentClass + '>)WITHNAME(dtor)' + backSlash + '\n')
    elif funcName == currentClass:
        noParams = ',' if len(gluedParameters) else ''
        out.write(
            ')::FUNC(CtorProvider<' + currentClass + noParams + gluedParameters + '>)WITHNAME(ctor)' + backSlash + '\n')
    else:
        out.write(')::' + virtual + 'FUNC(' + currentClass + ')' + 'WITHNAME(' + funcName + ')' + backSlash + '\n')

def parseString(string, out, backSlash):
    global bracketCounter
    if string == '{':
        bracketCounter += 1
    elif string == '}':
        bracketCounter -= 1
    elif bracketCounter <= 1 and regex.search(regIsVirtual, string) != None:
        parseMethod(string, out, backSlash, 'V')
    elif bracketCounter <= 1 and regex.search(regIsMethod, string) != None:
        parseMethod(string, out, backSlash)
    elif bracketCounter <= 1 and regex.search(regIsField, string) != None:
        parseField(string, out, backSlash)
    return bracketCounter == 0

def doSearch(pattern, string, it):
    match = regex.search(pattern, string[it:])
    if match:
        return match[0], match.end() + it
    else:
        return '', it

def parseClass(string, it, out):
    global currentClass
    global bracketCounter
    bracketCounter = 0
    temp_it = it
    currentClass, it = doSearch(regClassName, string, it)
    backSlash = ''
    isMacros = False
    if regex.search(regIsMacro, string[:temp_it]):
        isMacros = True
        backSlash = '\\'
        out.write(
            '#define TEMPLATE_' + currentClass + '(...)\\\n' + 'METAGEN_CLASS(' + currentClass + '<__VA_ARGS__>' + currentClass)
    else:
        out.write('METAGEN_CLASS(' + currentClass)
    inheritedClasses = string[it:][string[it:].find(':') + 1:string[it:].find('{')]
    inheritedClasses.lstrip();
    inheritedClasses.rstrip();
    for c in regex.findall(regInheritedClasses, inheritedClasses):
        out.write(',' + c)
    if isMacros:
        out.write(')\\\nWITH_MEMBERS(')
    else:
        out.write(')\nWITH_MEMBERS(')
    if isMacros:
        out.write('\\\n')
    else:
        out.write('\n')
    flag = False
    while flag == False:
        match, it = doSearch(regBracketOrFieldOrMethod, string, it)
        if not regex.search(r'NOT_METADATA|template\s*<', match):
            flag = parseString(match, out, backSlash)
    out.write(');\n')

def parseFile(file, out):
    f = open(file, 'r', -1, errors='ignore')
    string = ''.join([removeComments(line).rstrip() for line in f])
    for match in regex.finditer(lsclass, string):
        parseClass(string, match.end(), out)

os.system('pip install regex')
ignoredFileTypes, dirIgnore = getIgnore()
out = open(out_file, 'w', -1, errors='ignore')
out.write('void auto_init_metagen()\n{\n#ifdef BRGE_ENABLE_SERVER\n')
tree = os.walk(Path.cwd()) if len(sys.argv) < 2 else os.walk(sys.argv[1])
for root, dirs, files in tree:
    if in_directory(root, dirIgnore):
        continue
    for file in os.listdir(root):
        for end in endswith:
            if file.endswith(end):
                parseFile(root + '\\' + file, out)
out.write('#endif\n}\n')
out.close()
print('metadata was generated')