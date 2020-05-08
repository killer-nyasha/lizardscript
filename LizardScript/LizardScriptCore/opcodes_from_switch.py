import regex

input_file_name = 'RuntimeCases.i'
output_file1_name = 'Opcodes.hxx'
output_file2_name = 'OpcodesText.cxx'
output_file3_name = 'TypedOpcodes.hxx'

namespace_name = 'LsAsm'
enum_name = 'Opcodes'
map_name = 'textToOpcode'
function_name = 'makeTypedOpcodes'

find_opcodes_name = regex.compile(r'(?<=case ([\w]+::)?\s*)[^{};:]+\b(?!\s*::\s*)')
regUnnecessaryPatterns = regex.compile(r'/\*.*?\*/|#define.*?\n|//.*?\n|#include.*?\n|#line.*?\n|#pragma.*?\n'
r'|#if.*?#endif|#error.*?\n|#undef.*?\n',regex.DOTALL)

comma_enum, comma_map, comma_function = '','',''

def deleteUnnecessaryPatterns(string):
    return regex.sub(regUnnecessaryPatterns, " ", string)

def write_enum(out, opcode_name):
    global comma_enum
    out.write(comma_enum + opcode_name + '\n')
    comma_enum = ','

def write_in_map(out, opcode_name):
    global comma_map
    out.write(comma_map + '{"' + opcode_name + '", ' + 'LsAsm::' + opcode_name + '}\n')
    comma_map = ','

def write_typed(out, opcode_name):
    global comma_function
    opcode_split = opcode_name.split('_')
    out.write(comma_function + 'lsTyped("' + opcode_split[0] + '"')
    opcode_split.pop(0)
    for op in opcode_split:
        out.write(', typeInfo<' + op + '>()')
    out.write(', ' + 'LsAsm::' + opcode_name + ');\n')
    comma_function = ','

in_file = open(input_file_name,'r')
out_file1 = open(output_file1_name,'w')
out_file2 = open(output_file2_name,'w')
out_file3 = open(output_file3_name,'w')

out_file1.write('namespace '+ namespace_name + '\n{\nenum ' + enum_name + '\n{\n')
out_file2.write('#include "pch.h"\n#include "OpcodesText.h"\n#include "Opcodes.hxx"\nusing namespace LsAsm;\nconst char* opcodeToText[256];\nbool opcodes_initialized = false;\nstd::map<const char*, int, cmp_str> '+ map_name +'\n{\n')
out_file3.write('#include "pch.h"\n#include "TypedOpcodes.h"\n void ' + function_name + '()\n{\n')

string = ''.join([deleteUnnecessaryPatterns(line) for line in in_file])

for match in regex.finditer(find_opcodes_name, string):
    write_enum(out_file1, match[0].rstrip())
    write_in_map(out_file2, match[0].rstrip())
    write_typed(out_file3, match[0].rstrip())

out_file1.write('};\n}\n')
out_file2.write('};\n')
out_file3.write('};\n')

