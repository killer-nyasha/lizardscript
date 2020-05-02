import regex

# constants
#-------------------------------------------------------------------------------

# путь к входному файлу
inFilePath = 'opcodes.h'
# путь к выходному файлу
outFilePath = 'output.h'

# через | можно добавить новые ключевые слова
# (если речь идет о простых словах без параметров, вроде rnfirst/rnsecond)
simpleLsCppObjects = r'rnfirst|rnsecond'
complexLsCppObjects = r'CODEGET([^{};]*)'

regLsCppObjects = regex.compile(simpleLsCppObjects+r'|'+complexLsCppObjects)
regOpcodes = regex.compile(r'case\s+opcode::')
#-------------------------------------------------------------------------------

# перед первым LsCppOpcode запятая не должна ставиться
# (потом эта переменная поменяется)
firstComma = ''

# убираем ненужный код из строки
def deleteUnnecessaryPatterns(string):
    return regex.sub(regex.compile("\n|/\*.*?\*/|#define.*?\n|//.*?\n|#include.*?\n",regex.DOTALL ) ," " ,string)

# экранируем кавычки внутри блока case
def shielding(string,caseBlockPos):
    ret = string[:caseBlockPos]
    ret+=string[caseBlockPos:].replace('"',r'\"')
    return ret

# меняем простые LsCppObjects, например:
# rnfirst на LsCpp::rnfirst()
def replaceSimpleLsCppObjects(string):
    objects = simpleLsCppObjects.split(r'|')
    for o in objects:
        string = regex.sub(o,'LsCpp::'+o+r'()',string)
    return string

# меняем сложные LsCppObjects, например:
# CODEGET(...) на LsCpp::CODEGET(...)
def replaceComplexLsCppObjects(string):
    objects = complexLsCppObjects.split(r'|')
    for o in objects:
        for match in regex.finditer(o, string):
            string = string.replace(match.group(0),'LsCpp::'+match.group(0))
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

# главная функция, в которой мы из входящей строки генерируем новую
def genCode(string, position):
    global firstComma
    lsObjects = findLsCppObjects_(string)
    res = firstComma+'LsCppOpcode('
    firstComma = ','
    # пишем название опкода
    res+='opcode::'
    while string[position]!=':':
        res+=string[position]
        position+=1
    position += string[position:].find('{')+1
    if position == 0: raise Exception('input string error, can\'t find \'{\' symbol in case command')
    res+=', {^^{'
    caseBlockPos = len(res)
    bracketCounter = 1
    while bracketCounter !=0:
        while not isLsObject(position,lsObjects):
            if bracketCounter == 0: break
            if string[position] == '{':
                bracketCounter+=1
                position+=1
                continue
            elif string[position] == '}':
                bracketCounter -= 1
                position += 1
                continue
            res+=string[position]
            position += 1
        if bracketCounter != 0:
            object = getLsObjectByPosition(position,lsObjects)
            res += '^^,'+string[object[0]:object[1]]+',^^'
            position = object[1]
    res += '}^^})\n'
    res = replaceSimpleLsCppObjects(res)
    res = replaceComplexLsCppObjects(res)
    res = shielding(res,caseBlockPos)
    res = replaceQuotes(res)
    return res

# main
outFile = open(outFilePath, 'w', -1,errors='ignore')
inFile = open(inFilePath, 'r', -1, errors='ignore')
string = ''.join([deleteUnnecessaryPatterns(line).rstrip() for line in inFile])
for match in regex.finditer(regOpcodes, string):
    outFile.write(genCode(string,match.end()))



