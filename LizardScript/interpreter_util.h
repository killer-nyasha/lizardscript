#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "crossplatform_tchar.h"
#include <vector>
#include <stack>
#include <algorithm>
#include <string>
#include <typeindex>
#include <map>
#include <iostream>

#include "stringptr.h"
#include "Exception.h"

#include "metagen.h"

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

	//template <typename T>
	//inline int t_compare(const void* x1, const void* x2)
	//{
	//	return (*(T*)x1) - (*(T*)x2);
	//}
}