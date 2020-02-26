/*!
\file FastRemoving.h
\brief Compound container which provides O(1) addition and deletion
\author killer-nyasha
\version 0.2
\date 26.00.2020
*/
#pragma once
#include <array>
#include <vector>

//!it makes std::array interface similar to std::vector
//!so we can use std::array in some function without rewriting them for different containers, 
//!we should only add a template for container type
template <typename T, size_t arraySize>
class ArrayAsVectorWrapper
{
private:
	std::array<T, arraySize> data;
	size_t index;

public:
	void push_back(const T& elem)
	{
		data[index++] = elem;
	}

	size_t size()
	{
		return index;
	}

	size_t capacity()
	{
		return arraySize;
	}
};

//!undefining it will reduce FastRemoving's required memory a bit 
#define FR_ENABLE_FOREACH

//!Compound container which provides O(1) addition and deletion
//!and O(max size which this container had) iterating through all elements.
//!C - type of container.
template <typename C>
class FastRemoving
{
private:

	//!type of element
	using T = decltype (C())[0];

	//!unordered vector of objects
	std::vector<T> data;

	//!vector of free indices.
	//!when we erase object, its index just become free
	//!and the next added element will be placed at it
	std::vector<size_t> freeIndex;

public:

#ifdef FR_ENABLE_FOREACH
	//!flags which shows that some index isn't free
	std::vector<bool> bitflags;
#endif

	size_t push_back(const T& object)
	{
		size_t index = 0;
		if (freeIndex.size() == 0)
		{
			index = data.size();
			data.push_back(object);
#ifdef FR_ENABLE_FOREACH
			bitflags.push_back(true);
#endif
		}
		else
		{
			index = freeIndex[freeIndex.size() - 1];
			data[index] = object;
#ifdef FR_ENABLE_FOREACH
			bitflags[index] = true;
#endif
			freeIndex.pop_back();
		}
		return index;
	}

	void erase(size_t index)
	{
		data[freeIndex].~T();
		freeIndex.push_back(index);
#ifdef FR_ENABLE_FOREACH
		bitflags[freeIndex] = false;
#endif
	}

	T& operator[](size_t index)
	{
		return data[index];
	}

	const T& operator[](size_t index) const
	{
		return data[index];
	}
};

template <typename T>
using fr_vector = FastRemoving<std::vector<T>>;