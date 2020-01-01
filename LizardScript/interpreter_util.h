#pragma once

#include "crossplatform_tchar.h"
#include "stdafx.h"

#include "stringptr.h"
#include "Exception.h"

#include "Logger.h"
#include "metagen_classes.h"
#include "Print.h"

namespace LizardScript
{
	//!Linear vectorBinarySearch in vector
	//!\param[in] vec
	//!\param[in] elem
	//!\returns index of elem if it was found, else -1
	template <class T>
	int vectorBinarySearch(const std::vector<T>& vec, T elem)
	{
		//doesn't work with index > maxint32
		auto r = std::find(vec.begin(), vec.end(), elem) - vec.begin();
		return (r != vec.size() && vec[r] == elem) ? (int)r : -1;
	}

#define ALIAS(member, name) decltype(member)& name = member;

//#define COMPARATOR_LESS(type, ...) bool operator<(const type& other) const { return __VA_ARGS__; }
//#define COMPARATOR_EQ(type, ...) bool operator==(const type& other) const { return __VA_ARGS__; }
//#define OTHER_COMPARATORS(type)
}