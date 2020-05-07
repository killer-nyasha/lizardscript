#pragma once
#include "LizardScript.h"

#define REGISTER(type, i) reinterpret_cast<type*>(&stack[esp + i]) 
#define CODE(name) name
#define JMP(new_eip) goto lsaddr_##new_eip;

#define break

#include "LsTypedefs.h"