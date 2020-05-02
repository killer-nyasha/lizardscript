import regex
from os import system

# constants
#-------------------------------------------------------------------------------

# путь к входному файлу
inFilePath = 'RuntimeCases.i'
# путь к выходному файлу
outFilePath = 'LsCpp.hxx'

# через | можно добавить новые ключевые слова
# (если речь идет о простых словах без параметров, вроде rnfirst/rnsecond)
simpleLsCppObjects = r'r1|r2'
complexLsCppObjects = r'CODEGET([^{};]*)|REGISTER([^{};]*)'

# скомпилированные регулярки (для улучшения производительности)
regLsCppObjects = regex.compile(simpleLsCppObjects+r'|'+complexLsCppObjects)
regOpcodes = regex.compile(r'case\s+LsAsm::')
regUnnecessaryPatterns = regex.compile(r'public\s*:?|private\s*:?|protected\s*:?|\n|/\*.*?\*/|#define.*?\n|//.*?\n|#include.*?\n|#line.*?\n|#pragma.*?\n'
r'|#if.*?#endif|#error.*?\n|#undef.*?\n',regex.DOTALL)
#-------------------------------------------------------------------------------

# перед первым LsCppOpcode запятая не должна ставиться
# (потом эта переменная поменяется)
firstComma = ''

# убираем ненужный код из строки
def deleteUnnecessaryPatterns(string):
    return regex.sub(regUnnecessaryPatterns, " ", string)

# экранируем кавычки внутри блока case
def shielding(string,caseBlockPos):
    ret = string[:caseBlockPos]
    ret += string[caseBlockPos:].replace('"',r'\"')
    return ret

# меняем простые LsCppObjects, например:
# rnfirst на LsCpp::rnfirst()
def replaceSimpleLsCppObjects(string):
    objects = simpleLsCppObjects.split(r'|')
    for o in objects:
        string = regex.sub(o,'this->'+o+r'()',string)
    return string

# меняем сложные LsCppObjects, например:
# CODEGET(...) на LsCpp::CODEGET(...)
def replaceComplexLsCppObjects(string):
    objects = complexLsCppObjects.split(r'|')
    for o in objects:
        for match in regex.finditer(o, string):
            string = string.replace(match.group(0),'this->'+match.group(0))
    return string

# меняем ^^ на кавычки (так обозначаются кавычки, которые не нужно экранировать)
def replaceQuotes(string):
    return string.replace(r'^^','"')

# находим LsCppObjects
def findLsCppObjects_(string):
    return tuple((x.start(),x.end()) for x in regex.finditer(regLsCppObjects, string))

# получаем LsCppObjects по позиции
def getLsObjectByPosition(position,lsObjects):
    for o in lsObjects:
        if o[0] == position:
            return o
    raise Exception('object doesn\'t exist on this position')

# проверяем не попали ли мы на LsCppObject
def isLsObject(position,lsObjects):
    for o in lsObjects:
        if o[0] == position:
            return True
    return False

# записываем в результат название опкода
def writeOpcodeName(string,position,res):
    res += 'LsAsm::'
    while string[position] != ':':
        res += string[position]
        position += 1
    return res,position

# функция для форматирования строки (экранирование кавычек,
# добавления пространств имен у LsCppObjects и т.д.
def stringFormatting(string,caseBlockPos):
    string = replaceSimpleLsCppObjects(string)
    string = replaceComplexLsCppObjects(string)
    string = shielding(string,caseBlockPos)
    string = replaceQuotes(string)
    return string

# записываем в результат блок case
def writeCaseBlock(string,position,res,lsObjectsPositions):
    bracketCounter = 1
    while bracketCounter != 0:
        while not isLsObject(position, lsObjectsPositions):
            if bracketCounter == 0: break
            if string[position] == '{':
                bracketCounter += 1
                position += 1
                continue
            elif string[position] == '}':
                bracketCounter -= 1
                position += 1
                continue
            res += string[position]
            position += 1
        if bracketCounter != 0:
            object = getLsObjectByPosition(position, lsObjectsPositions)
            res += '^^,' + string[object[0]:object[1]] + ',^^'
            position = object[1]
    return res

# главная функция, в которой мы из входящей строки генерируем новую
def genCode(string, position):
    global firstComma
    lsObjectsPositions = findLsCppObjects_(string)
    res = firstComma+'LsCppOpcode('
    firstComma = ','
    res,position = writeOpcodeName(string,position,res)
    position += string[position:].find('{') + 1
    if position == 0: raise Exception('input string error, can\'t find \'{\' symbol in case command')
    res+=', {^^{'
    # здесь начинается блок case, кавычки должны экранироваться
    caseBlockPos = len(res)
    res = writeCaseBlock(string,position,res,lsObjectsPositions)
    res += '}^^})\n'
    return stringFormatting(res,caseBlockPos)

# main

#system('pip install regex')
outFile = open(outFilePath, 'w', -1,errors='ignore')
inFile = open(inFilePath, 'r', -1, errors='ignore')
string = ''.join([deleteUnnecessaryPatterns(line).rstrip() for line in inFile])
for match in regex.finditer(regOpcodes, string):
    outFile.write(genCode(string,match.end()))



