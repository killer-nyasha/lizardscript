#pragma once
namespace LsOpcode
{ 
	enum LsOpcode
	{
		nop = 0,
		set_32,
		set_64,
		set_big,

		push_32,
		push_64,
		push_this,
		push_offset,
		set_this,
		mov,
		get_32,
		get_64,
		push_stringptr,
		set_stringptr,

		push_stackptr,
		push_stackbase,
		set_stackptr,

		add_int_int,
		sub_int_int,
		mul_int_int,
		div_int_int,

		add_float_float,
		sub_float_float,
		mul_float_float,
		div_float_float,

		int_to_float,
		float_to_int,

		inc,
		dec,
		//_opt_push_this_and_offset,
		//_opt_set_this_and_offset,

		jmp,
		jz,

		more_int_int,
		less_int_int,

		call_cpp,

		alloc,
		free,

		comment,
		ret,

		push_lsl,
	};
}