import regex

input_file_name = 'RuntimeCases.i'
output_file1_name = 'Opcodes.hxx'
output_file2_name = 'OpcodesText.cxx'

namespace_name = 'LsAsm'
enum_name = 'Opcodes'
map_name = 'textToOpcode'

find_opcodes_name = regex.compile(r'(?<=case ([\w]+::)?\s*)[^{};:]+\b(?!\s*::\s*)')
regUnnecessaryPatterns = regex.compile(r'/\*.*?\*/|#define.*?\n|//.*?\n|#include.*?\n|#line.*?\n|#pragma.*?\n'
r'|#if.*?#endif|#error.*?\n|#undef.*?\n',regex.DOTALL)

comma_enum, comma_map = '',''

def deleteUnnecessaryPatterns(string):
    return regex.sub(regUnnecessaryPatterns, " ", string)

def write_enum(out, opcode_name):
    global comma_enum
    out.write(comma_enum + opcode_name + '\n')
    comma_enum = ','

def write_in_map(out, opcode_name):
    global comma_map
    out.write(comma_map + '{"' + opcode_name + '", ' + opcode_name + '}\n')
    comma_map = ','

in_file = open(input_file_name,'r')
out_file1 = open(output_file1_name,'w')
out_file2 = open(output_file2_name,'w')

out_file1.write('namespace '+ namespace_name + '\n{\nenum ' + enum_name + '\n{\n')
out_file2.write('#include "pch.h"\n#include "OpcodesText.h"\n#include "Opcodes.hxx"\nusing namespace LsAsm;\nconst char* opcodeToText[256];\nstd::map<const char*, int> '+ map_name +'\n{\n')

string = ''.join([deleteUnnecessaryPatterns(line) for line in in_file])

for match in regex.finditer(find_opcodes_name, string):
    write_enum(out_file1, match[0].rstrip())
    write_in_map(out_file2,match[0].rstrip())

out_file2.write('};\n')
out_file1.write('};\n}\n')

