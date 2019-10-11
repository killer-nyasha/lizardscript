#pragma once

using regindex = unsigned char;

struct typed_reg
{
	regindex index;
	TypeInfo type;

	typed_reg()
	{

	}

	typed_reg(regindex index, TypeInfo type) : index(index), type(type)
	{

	}
};

struct regindex_pair
{
	regindex first : 4;
	regindex second : 4;

	regindex_pair()
	{ }

	regindex_pair(regindex rn)
	{
		*this = *reinterpret_cast<regindex_pair*>(&rn);
	}

	regindex_pair(regindex r1, regindex r2) : first(r1), second(r2)
	{

	}

	regindex_pair(typed_reg rn)
	{
		*this = *reinterpret_cast<regindex_pair*>(&rn.index);
	}

	regindex_pair(typed_reg r1, typed_reg r2) : first(r1.index), second(r2.index)
	{

	}

	regindex get()
	{
		return *reinterpret_cast<regindex*>(this);
	}

	template <typename T>
	friend T& operator<<(T& stream, regindex_pair p)
	{
		stream << "[" << (int)p.first << ", " << (int)p.second << "]";
		return stream;
	}
};

//template <typename T>
class RuntimeData;

//!function which run when interpreter meets certain operator
//!\param[in] RuntimeData
using KeywordAction = void(*)(regindex_pair, RuntimeData&);
