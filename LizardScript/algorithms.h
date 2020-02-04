/*!
\file algorithms.h
\brief Quick sort and binary search. There will be other useful algorithms in future.
\author all algorithms were stolen by killer-nyasha ^_^
\version 0.2
\date 17.01.2020
*/
#pragma once
#include <vector>

namespace Impl
{ 
	template <typename T> 
	inline size_t MedianOfThree(T &a, T &b, T &c, size_t ai, size_t bi, size_t ci)
	{
		if (a >= b && a <= c)
			return ai;
		if (b >= a && b <= c)
			return bi;
		return ci;
	}

	template <typename T> 
	inline size_t QuicksortPartition(std::vector<T> &A, size_t lo, size_t hi)
	{
		size_t ci = (int)(0.5*(lo + hi) + 0.5);
		size_t pivotIndex = MedianOfThree(A[lo], A[ci], A[hi], lo, ci, hi);
		T pivot = A[pivotIndex];

		size_t i = lo;
		size_t j = hi;
		while (true)
		{
			while (A[i] <= pivot && i < pivotIndex) i++;
			while (A[j] >= pivot && j > pivotIndex) j--;
			if (i == pivotIndex && j == pivotIndex)
				return pivotIndex;

			T tmp = A[i];
			A[i] = A[j];
			A[j] = tmp;

			if (i == pivotIndex)
				pivotIndex = j;
			else if (j == pivotIndex)
				pivotIndex = i;
		}
	}

	template <typename T>
	inline size_t pQuicksortPartition(std::vector<T>& A, size_t lo, size_t hi)
	{
		size_t ci = (int)(0.5 * (lo + hi) + 0.5);
		size_t pivotIndex = MedianOfThree(*A[lo], *A[ci], *A[hi], lo, ci, hi);
		T pivot = A[pivotIndex];

		size_t i = lo;
		size_t j = hi;
		while (true)
		{
			while (*A[i] <= *pivot && i < pivotIndex) i++;
			while (*A[j] >= *pivot && j > pivotIndex) j--;
			if (i == pivotIndex && j == pivotIndex)
				return pivotIndex;

			T tmp = A[i];
			A[i] = A[j];
			A[j] = tmp;

			if (i == pivotIndex)
				pivotIndex = j;
			else if (j == pivotIndex)
				pivotIndex = i;
		}
	}

	template <typename T> 
	inline void Quicksort(std::vector<T> &A, size_t lo, size_t hi)
	{
		if (lo < hi)
		{
			size_t p = QuicksortPartition<T>(A, lo, hi);
			if (p > 0) //p is unsigned, so prevent negative
				Quicksort<T>(A, lo, p - 1);
			Quicksort<T>(A, p + 1, hi);
		}
	}

	template <typename T>
	inline void pQuicksort(std::vector<T>& A, size_t lo, size_t hi)
	{
		if (lo < hi)
		{
			size_t p = pQuicksortPartition<T>(A, lo, hi);
			if (p > 0) //p is unsigned, so prevent negative
				pQuicksort<T>(A, lo, p - 1);
			pQuicksort<T>(A, p + 1, hi);
		}
	}
}

//!Quick sort of a vector
//!\param[in] A Vector
//!\warning Operators >= and <= required
template <typename T> 
inline void quickSort(std::vector<T> &A)
{
	if (A.size() <= 1)
		return; 
	Impl::Quicksort(A, 0, A.size() - 1);
}

//!Quick sort of a vector of pointers
//!\param[in] A Vector
//!\warning Operators >= and <= required
template <typename T>
inline void pQuickSort(std::vector<T>& A)
{
	if (A.size() <= 1)
		return;
	Impl::Quicksort(A, 0, A.size() - 1);
}

//!Binary search in a vector
//!\param[in] a Vector
//!\param[in] x Element
//!\warning Operators < and == required
//!\returns index of elem if it's found, else -1
template <typename T>
inline int binarySearch(const std::vector<T>& a, const T& x)
{
	if (a.size() == 0)
		return -1;

	int left = 0;
	int right = static_cast<int>(a.size()) - 1;
	while (left <= right)
	{
		int middle = (left + right) / 2;
		if (a[middle] == x)
		{
			return (int)middle;
		}
		if (a[middle] < x)
		{
			left = middle + 1;
		}
		else
		{
			right = middle - 1;
		}
	}

	return -1;
}

//!Binary search in a vector of pointers
//!\param[in] a Vector
//!\param[in] x Element
//!\warning Operators < and == required
//!\returns index of elem if it's found, else -1
template <typename T>
inline int pBinarySearch(const std::vector<T>& a, const T& x)
{
	if (a.size() == 0)
		return -1;

	int left = 0;
	int right = static_cast<int>(a.size()) - 1;
	while (left <= right)
	{
		int middle = (left + right) / 2;
		if (*a[middle] == *x)
		{
			return (int)middle;
		}
		if (*a[middle] < *x)
		{
			left = middle + 1;
		}
		else
		{
			right = middle - 1;
		}
	}

	return -1;
}

//!Find range in which elements in sorted vector are equal to some element
//!\param[in] a Sorted ector
//!\param[in] x index of element
//!\warning Operator == required
//!\returns for example, for { 1, 2, 2, 2, 3 } and index 1 it returns (1, 3) - because elements at 1,2,3 are equal to element at 1
template <typename T>
inline std::pair<int, int> findRange(const std::vector<T>& a, int x)
{
	size_t left = x, right = x;

	for (; left + 1 >= 1; left--)
		if (!(a[left] == x))
			break;
	for (; right + 1 >= 1; left--)
		if (!(a[left] == x))
			break;
	return std::make_pair(left, right);
}